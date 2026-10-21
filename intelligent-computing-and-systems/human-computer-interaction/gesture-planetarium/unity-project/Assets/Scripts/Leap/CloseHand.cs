using UnityEngine;
using UnityEngine.UI;
using Leap;

public class CloseHand : MonoBehaviour
{
    public LeapServiceProvider provider;
    public float grabThreshold = 0.8f;
    public float releaseThreshold = 0.3f;

    public LeapPlanetSelector planetSelector;

    private bool isClosed = false;

    void Update()
    {
        if (provider == null) return;

        Frame frame = provider.CurrentFrame;
        foreach (Hand hand in frame.Hands)
        {
            float grab = hand.GrabStrength;

            if (!isClosed && grab > grabThreshold)
            {
                isClosed = true;
                OnFistClosed(hand);
            }
            else if (isClosed && grab < releaseThreshold)
            {
                isClosed = false;
                OnFistOpened(hand);
            }
        }
    }

    void OnFistClosed(Hand hand)
    {
        Debug.Log($"✊ Mano {(hand.IsLeft ? "izquierda" : "derecha")} cerrada");
        planetSelector.TrySelectPlanet();

    }
    void OnFistOpened(Hand hand)
    {
        Debug.Log($"🖐️ Mano { (hand.IsLeft ? "izquierda" : "derecha") } abierta");
    }
}

