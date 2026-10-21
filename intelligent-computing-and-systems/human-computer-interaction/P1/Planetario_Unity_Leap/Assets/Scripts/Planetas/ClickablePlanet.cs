using UnityEngine;
using UnityEngine.EventSystems;

/*****************************************************************/
//Esta clase permite que un planeta sea clickeable mediante eventos
//de UI.
// Al hacer clic, avisa al GameManager para que realice el zoom hacia
// ese planeta concreto.
// Da un ID interno del planeta para identificarlo en otros sistemas
// (CSV, datos, etc.).
/*****************************************************************/
public class PlanetClickable : MonoBehaviour, IPointerClickHandler
{
    [Header("Datos del Planeta")]
    public float zoomDistance = 5f;
    public string displayName;

    [SerializeField] private string _id_planeta;


    public void OnPointerClick(PointerEventData eventData)
    {
        // 1. Busca el controlador de la cámara (que está en la cámara)
        if (GameManager.Instance == null)
        {
            Debug.LogError("No se encuentra un GameManager en la escena.");
            return;
        }

        // 2. Le pide al controlador que haga zoom, pasándole
        //    una referencia a este mismo script de planeta.
        GameManager.Instance.RequestZoom(this);
    }

    public string GetId ()
    {
        return _id_planeta;
    }
}