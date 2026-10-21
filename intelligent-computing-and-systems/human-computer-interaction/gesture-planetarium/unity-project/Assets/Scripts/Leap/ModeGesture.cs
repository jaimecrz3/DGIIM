using UnityEngine;
using Leap;

/************************************************************/
//Esta clase detecta poses de mano con Leap Motion para cambiar
//el modo de juego (Kid, Normal, Expert).
// Comprueba orientación de la palma, dedos extendidos y exige
// mantener la pose un tiempo mínimo.
// Aplica cooldown entre detecciones y notifica el modo elegido
// al GameManager. 
/************************************************************/
public class PoseGestureDetector : MonoBehaviour
{
    [Header("Leap")]
    public LeapServiceProvider provider; // Leap provider

    [Header("Umbrales")]
    [Tooltip("Tiempo mínimo (s) que la pose debe mantenerse")]
    public float holdSeconds = 0.45f;
    [Tooltip("Tiempo de enfriamiento tras aceptar una pose")]
    public float cooldown = 1.0f;
    [Tooltip("La palma debe mirar ~hacia la cámara (1=de frente, 0=de canto)")]
    [Range(0f, 1f)] public float palmFacingMinDot = 0.7f;

    [Header("Mano")]
    public bool onlyLeftHand = true;

    private float poseTimer = 0f;
    private float lastAcceptTime = -999f;

    void Update()
    {
        if (provider == null) return;
        Frame frame = provider.CurrentFrame;
        if (frame == null || frame.Hands.Count == 0) { poseTimer = 0f; return; }

        // Elige una mano (por simplicidad, la primera abierta)
        Hand hand = null;
        foreach (var h in frame.Hands)
        {
            if (onlyLeftHand && !h.IsLeft) continue;
            if (h.GrabStrength < 0.6f) { hand = h; break; }
        }
        if (hand == null) { poseTimer = 0f; return; }

        // Filtrado: que la palma mire (más o menos) a la cámara
        var palmDir = new Vector3(hand.PalmNormal.x, hand.PalmNormal.y, hand.PalmNormal.z); // normal de la palma
        var camForward = Camera.main.transform.forward;
        float facing = Vector3.Dot(-palmDir.normalized, camForward.normalized); // palma hacia cámara
        if (facing > palmFacingMinDot) { poseTimer = 0f; return; }

        // Lee dedos extendidos
        bool thumb = hand.GetFinger(Finger.FingerType.THUMB).IsExtended;
        bool index = hand.GetFinger(Finger.FingerType.INDEX).IsExtended;
        bool middle = hand.GetFinger(Finger.FingerType.MIDDLE).IsExtended;
        bool ring = hand.GetFinger(Finger.FingerType.RING).IsExtended;
        bool pinky = hand.GetFinger(Finger.FingerType.PINKY).IsExtended;

        // Clasifica la pose
        GameMode? pose = null;
        if (!thumb && index && middle && !ring && !pinky) pose = GameMode.Kid;     // V / peace
        else if (thumb && index && middle && ring && pinky) pose = GameMode.Normal;  // mano abierta
        else if (!thumb && !index && middle && ring && pinky) pose = GameMode.Expert;  // tres dedos

        // Si no estamos en ninguna pose válida, resetea temporizador
        if (pose == null) { poseTimer = 0f; return; }

        // Mantener pose un tiempo
        poseTimer += Time.deltaTime;
        if (poseTimer < holdSeconds) return;

        // Cooldown para no disparar varias veces
        if (Time.time - lastAcceptTime < cooldown) return;
        lastAcceptTime = Time.time;
        poseTimer = 0f;

        // Gesto aceptado
        GameManager.Instance?.SetMode(pose.Value);
    }
}

