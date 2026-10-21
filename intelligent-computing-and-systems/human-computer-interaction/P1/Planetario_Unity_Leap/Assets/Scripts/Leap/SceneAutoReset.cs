using Leap;
using UnityEngine;
using UnityEngine.SceneManagement; // Necesario para reiniciar la escena
using UnityEngine.XR;

/********************************************************************/
//Esta clase controla si hay manos en el Leap y resetea la escena por
//inactividad. Cada Update de consulta el CurrentFrame del
//LeapServiceProvider.
//También desactiva el panel StandBy la primera vez que se detectan las
//manos y a partir de ese momento ai no hay manos va incrementando 
//currentTimer hasta que llega a timeToReset, y se reinicia la escena
/********************************************************************/
public class SceneAutoReset : MonoBehaviour
{
    [Header("Configuración Leap")]
    public LeapServiceProvider provider;

    [Header("Configuración de Reinicio")]
    [Tooltip("Tiempo en segundos sin manos detectadas antes de reiniciar.")]
    public float timeToReset = 5.0f;

    [Header("Debug (Solo lectura)")]
    [SerializeField] private float currentTimer = 0f;

    private bool manosDetectadasPrimeraVez = false;

    void Update()
    {
        if (provider == null) return;

        // Obtenemos el frame actual
        Frame frame = provider.CurrentFrame;

        // Verificamos si hay ALGUNA mano visible (Count > 0)
        if (frame != null && frame.Hands.Count > 0)
        {
            // Si hay manos, reseteamos el temporizador a 0
            currentTimer = 0f;

            if (!manosDetectadasPrimeraVez)
            {
                manosDetectadasPrimeraVez = true;
                GameManager.Instance.SalirStandBy();
            }
        }
        else
        {
            if (GameManager.Instance.GetState() == GameStates.StandBy) return;

            // Si NO hay manos, empezamos a sumar tiempo
            currentTimer += Time.deltaTime;

            // Si el tiempo supera el límite establecido
            if (currentTimer >= timeToReset)
            {
                Debug.Log("Tiempo de inactividad excedido. Reiniciando escena...");
                GameManager.Instance.ResetScene();
            }
        }
    }
}