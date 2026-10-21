using UnityEngine;
using Leap;

/*********************************************************************/
//Esta clase detecta un gesto de salida levantando ambas palmas hacia
//arriba con Leap Motion.
//Comprueba que se vean las dos manos, con la palma orientada hacia arriba,
//y exige mantener la pose cierto tiempo.
//Al completar el tiempo configurado, cierra la aplicación.
/*********************************************************************/
public class ExitGesture : MonoBehaviour
{
    public LeapProvider leapProvider; 

    [Tooltip("Tiempo necesario manteniendo el gesto para salir")]
    public float holdTimeParams = 3.0f;

    private float currentHoldTime = 0f;
    private bool isGestureActive = false;

    void Update()
    {
        if (leapProvider == null) return;

        Frame frame = leapProvider.CurrentFrame;

        // Condición 1: Tienen que verse las DOS manos
        if (frame.Hands.Count == 2)
        {
            bool bothPalmsUp = true;

            foreach (Hand hand in frame.Hands)
            {
                // La normal de la palma es un vector perpendicular a la palma.
                // Si (0, 1, 0) es arriba, buscamos que se parezca a eso.
                // Un valor > 0.7f significa que está bastante inclinada hacia arriba.
                if (hand.PalmNormal.y < 0.7f)
                {
                    bothPalmsUp = false;
                    break;
                }
            }

            if (bothPalmsUp)
            {
                ProcesarGesto();
            }
            else
            {
                ResetGesto();
            }
        }
        else
        {
            ResetGesto();
        }
    }

    void ProcesarGesto()
    {
        if (!isGestureActive)
        {
            isGestureActive = true;
            Debug.Log("Gesto de salida detectado... Mantén la pose.");
        }

        currentHoldTime += Time.deltaTime;

        if (currentHoldTime >= holdTimeParams)
        {
            SalirDeLaApp();
        }
    }

    void ResetGesto()
    {
        currentHoldTime = 0f;
        isGestureActive = false;
    }

    void SalirDeLaApp()
    {
        Debug.Log("!!! SALIENDO DE LA APLICACIÓN !!!");

        Application.Quit();

        #if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
        #endif
    }
}