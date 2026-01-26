using UnityEngine;
using Leap;

/***************************************************************/
// Esta clase detecta gestos de swipe con la mano derecha usando
// Leap Motion.
// Analiza la velocidad de la palma para detectar swipes horizontales
// y hacia atrás.
// Notifica los gestos detectados al GameManager aplicando umbral
// de velocidad y cooldown.
/***************************************************************/
public class SwipeGestureDetector : MonoBehaviour
{
    public LeapServiceProvider provider;

    [Header("Parámetros del gesto")]
    public float swipeSpeed = 1.5f;           // velocidad mínima (m/s)
    public float limitTangent = 0.6f;         // proporción para filtrar gestos en otros ejes
    public float cooldown = 1.0f;             // tiempo mínimo entre gestos (segundos)
    public float maxGrabStrength = 0.3f;      // mano abierta (no puño)

    private float lastSwipeTime = 0f;

    void Update()
    {
        if (provider == null) return;

        Frame frame = provider.CurrentFrame;
        if (frame == null || frame.Hands.Count == 0) return;

        // SOLO MANO DERECHA
        Hand rightHand = frame.Hands.Find(h => h.IsRight);
        if (rightHand == null) return;

        // Solo manos abiertas
        if (rightHand.GrabStrength < maxGrabStrength)
        {
            DetectSwipe(rightHand);
        }
    }

    void DetectSwipe(Hand hand)
    {
        // Evitar repetición por cooldown
        if (Time.time - lastSwipeTime < cooldown)
            return;

        // Obtenemos las velocidades en los 3 ejes
        float vx = hand.PalmVelocity.x;
        float vy = hand.PalmVelocity.y;
        float vz = hand.PalmVelocity.z;

        // Calculamos los valores absolutos para comparar magnitudes
        float absX = Mathf.Abs(vx);
        float absY = Mathf.Abs(vy);
        float absZ = Mathf.Abs(vz);

        // --- DETECCIÓN HORIZONTAL (Izquierda / Derecha) ---
        // El movimiento en X debe ser mayor que la velocidad mínima
        // Y debe ser el movimiento dominante (mayor que Y y mayor que Z)
        if (absX > swipeSpeed && absX > absY && absX > absZ)
        {
            if (absY < limitTangent * absX && absZ < limitTangent * absX)
            {
                if (vx > 0)
                {
                    GameManager.Instance.HandleSwipeLeft(hand);
                    Debug.Log("➡️ Swipe Derecha");
                }
                else
                {
                    GameManager.Instance.HandleSwipeRight(hand);
                    Debug.Log("⬅️ Swipe Izquierda");
                }
                lastSwipeTime = Time.time;
            }
        }

        // --- DETECCIÓN PROFUNDIDAD (Atrás) ---
        // El movimiento en Z debe ser mayor que la velocidad mínima
        // Y debe ser dominante sobre X e Y
        else if (absZ > swipeSpeed && absZ > absY && absZ > absX)
        {
            // Filtro extra
            if (absY < limitTangent * absZ && absX < limitTangent * absZ)
            {

                if (vz < 0) 
                {
                    GameManager.Instance.HandleSwipeBack(hand); 
                    Debug.Log("⬇️ Swipe Atrás (Pull)");

                    lastSwipeTime = Time.time;
                }
                
            }
        }
    }
}