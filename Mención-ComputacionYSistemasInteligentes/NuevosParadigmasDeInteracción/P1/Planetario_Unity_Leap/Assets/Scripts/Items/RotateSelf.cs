using UnityEngine;

/********************************************************************/
//Esta clase aplica un movimiento continuo al objeto al que
//está asociado. En cada Update gira el objeto alrededor del eje Y
//en espacio mundial a una velocidad constante.
//Hace que el objeto rote sobre sí mismo mientras se inclina
//suavemente, dando sensación de movimiento flotante.
/********************************************************************/
public class RotateSelf : MonoBehaviour
{
    [Header("Rotación continua")]
    public float rotationSpeedY = 15f;  // velocidad de rotación en Y

    [Header("Oscilación suave")]
    public float tiltAmplitudeX = 5f;
    public float tiltAmplitudeZ = 3f;
    public float tiltSpeed = 1f;        // velocidad del balanceo

    private Quaternion baseRotation;

    void Start()
    {
        baseRotation = transform.rotation; // guarda la rotación inicial
    }

    void Update()
    {
        // Rotación continua sobre el eje Y
        transform.Rotate(Vector3.up, rotationSpeedY * Time.deltaTime, Space.World);

        // Cálculo de las oscilaciones (seno y coseno)
        float tiltX = Mathf.Sin(Time.time * tiltSpeed) * tiltAmplitudeX;
        float tiltZ = Mathf.Cos(Time.time * tiltSpeed * 0.8f) * tiltAmplitudeZ;

        // Aplica la inclinación sobre la rotación actual
        transform.rotation = transform.rotation * Quaternion.Euler(tiltX * Time.deltaTime, 0f, tiltZ * Time.deltaTime);
    }
}
