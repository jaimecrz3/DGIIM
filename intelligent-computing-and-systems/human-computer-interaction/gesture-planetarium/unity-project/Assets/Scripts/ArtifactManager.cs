using UnityEngine;

public class ArtifactManager : MonoBehaviour
{
    [Header("Referencia al controlador de zoom (cámara)")]
    public PlanetZoomController zoomController;

    [Header("Artefactos en el mismo orden que la lista de planetas")]
    public GameObject[] artefactos;

    void Start()
    {
        // Ocultar todo al inicio
        OcultarTodos();
        zoomController.OnZoomCompleted += HandleZoomCompleted;
    }

    void HandleZoomCompleted(bool zoomIn)
    {
        if (!zoomIn)
        {
            // Zoom Out -> Vista general → ocultar todos
            OcultarTodos();
            return;
        }

        // Zoom In -> mostrar el que toca
        int index = zoomController.planets.IndexOf(zoomController.currentTarget);
        if (index >= 0 && index < artefactos.Length)
        {
            for (int i = 0; i < artefactos.Length; i++)
                artefactos[i].SetActive(i == index);
        }
    }

    void OcultarTodos()
    {
        foreach (var a in artefactos)
            a.SetActive(false);
    }
}