using NUnit.Framework;
using System;
using System.Collections;
using System.Collections.Generic;
//using System.Numerics;
using UnityEngine;
using UnityEngine.UIElements;

/**********************************************************************/
//Esta clase se se encarga de controlar la cámara cuando se hace zoom
//sobre los planetas y al navegar entre ellos. Guarda la posición y
//rotación por defecto de la cámara y un planeta objetivo actual
//(currentTarget), más una lista ordenada de planetas (planets) para
//moverse entre vecinos. 
/**********************************************************************/
public class PlanetZoomController : MonoBehaviour
{
    [Header("Configuración de Zoom")]
    public float zoomDuration = 1.5f; // Duración global del zoom

    private Camera cam;
    private Vector3 defaultCamPos;
    private Quaternion defaultCamRot;

    private bool isZooming = false;
    public PlanetClickable currentTarget;

    public event Action<bool> OnZoomCompleted; // true = zoom in, false = zoom out
    private bool lastZoomIn;                   // recordamos qué acción estamos haciendo

    [Header("Planets (izq → der)")]
    public List<PlanetClickable> planets = new();
   
    void Start()
    {
        cam = Camera.main;
        if (cam != null)
        {
            defaultCamPos = cam.transform.position;
            defaultCamRot = cam.transform.rotation;
        }
    }

    //--------------------------------------------------------------------------------------//
    //Esta método hace lo siguiente. Recibe un planeta. Si ya está haciendo zoom, no hace nada.
    //Si el planeta pedido es el que ya está seleccionado, realiza zoom out: vuelve la cámara a
    //su posición inicial.
    //Si es un planeta nuevo, calcula el nueva posición de la camara usando datos del planeta,
    //orienta la cámara hacia él y lanza un zoom in.
    //--------------------------------------------------------------------------------------//
    public bool? RequestZoom(PlanetClickable planet)
    {
        if (isZooming) return null;

        if (currentTarget == planet)
        {
            Debug.Log("Zoom Out");
            currentTarget = null;
            lastZoomIn = false;
            StartCoroutine(MoveCamera(defaultCamPos, defaultCamRot));
            return false;
        }
        else
        {
            currentTarget = planet;


            // Calcula el destino usando los datos del planeta
            float radius = 1f;
            MeshRenderer rend = planet.GetComponent<MeshRenderer>();
            if (rend != null) { radius = rend.bounds.extents.magnitude; }

            Vector3 sideDirection = Vector3.right;
            float safeDistance = radius * 2.5f + planet.zoomDistance;
            Vector3 targetPos = planet.transform.position + sideDirection * safeDistance;

            Vector3 lookPos = planet.transform.position;
            lookPos.y -= radius * 1.3f;
            Quaternion targetRot = Quaternion.LookRotation(lookPos - targetPos);

            // Inicia el movimiento
            lastZoomIn = true;
            StartCoroutine(MoveCamera(targetPos, targetRot));

            return true;
        }
    }

    //--------------------------------------------------------------------------------------//
    //Esta método hace lo siguiente. Permite ir al planeta vecino (izquierda/derecha) y pedir
    //automáticamente el zoom a ese planeta, con opción de hacer wrap-around al principio/fin.
    //--------------------------------------------------------------------------------------//
    public void SelectNeighbor(int dir, bool wrap = false)
    {
        if (currentTarget == null || planets == null || planets.Count == 0 || isZooming) return;

        int i = planets.IndexOf(currentTarget);
        if (i < 0) return;

        int next = i + dir;

        if (wrap)
        {
            if (next < 0) next = planets.Count - 1;
            if (next >= planets.Count) next = 0;
        }
        else
        {
            next = Mathf.Clamp(next, 0, planets.Count - 1);
            if (next == i) return;
        }

        RequestZoom(planets[next]); // Zoom al vecino; GameManager actualizará la UI en OnZoomCompleted
    }

    private IEnumerator MoveCamera(Vector3 targetPos, Quaternion targetRot)
    {
        isZooming = true;
        float timer = 0f;

        Vector3 startPos = cam.transform.position;
        Quaternion startRot = cam.transform.rotation;

        while (timer < zoomDuration)
        {
            timer += Time.deltaTime;
            float t = Mathf.Clamp01(timer / zoomDuration);
            t = t * t * (3f - 2f * t); // Suavizado

            cam.transform.position = Vector3.Lerp(startPos, targetPos, t);
            cam.transform.rotation = Quaternion.Slerp(startRot, targetRot, t);

            yield return null;
        }

        cam.transform.position = targetPos;
        cam.transform.rotation = targetRot;
        isZooming = false;

        OnZoomCompleted?.Invoke(lastZoomIn);
    }


}