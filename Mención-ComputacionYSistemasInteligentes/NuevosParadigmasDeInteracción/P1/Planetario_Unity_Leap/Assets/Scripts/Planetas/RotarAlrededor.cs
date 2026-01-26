using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/**********************************************************/
//Esta clase hace que un objeto gire alrededor de otro
//objeto en la escena.
/**********************************************************/

public class RotateAround : MonoBehaviour
{
    public GameObject objeto;
    public float rotationSpeed = 50.0f;
    

    // Update se llama una vez por frame
    void Update()
    {
        transform.RotateAround(objeto.transform.position, objeto.transform.up, rotationSpeed * Time.deltaTime);

    }
}