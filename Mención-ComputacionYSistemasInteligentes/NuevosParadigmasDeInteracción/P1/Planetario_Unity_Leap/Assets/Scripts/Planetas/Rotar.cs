using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*****************************************************************/
//Esta clase hace girar continuamente el objeto al que está asociada.
//Aplica una rotación en el eje Y en cada frame usando rotationSpeed.
/*****************************************************************/

public class Rotar : MonoBehaviour
{

    public float rotationSpeed = 50.0f;
    
    void Update()
    {

        transform.Rotate(0, rotationSpeed * Time.deltaTime, 0);

    }

}