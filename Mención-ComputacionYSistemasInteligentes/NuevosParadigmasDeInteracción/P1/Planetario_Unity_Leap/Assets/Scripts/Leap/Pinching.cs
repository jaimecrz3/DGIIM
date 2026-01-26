using Leap;
using System;
using System.Collections.Generic;
using System.Net.NetworkInformation;
//using System.Numerics;
using UnityEditor;
using UnityEditor.Experimental.GraphView;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;          

/*****************************************************************************/
//Esta clase convierte el gesto de pinch con la mano derecha (Leap) en “clicks”
//sobre la interfaz y sobre los planetas.
//Esta clase permite seleccionar tanto botones de interfaz como planetas 3D usando
//únicamente el gesto de pinza.
/*****************************************************************************/
public class LeapPlanetSelector : MonoBehaviour
{
    [Header("Referencias")]
    public LeapServiceProvider leapProvider;
    public Camera mainCamera;
    public RectTransform pointerUI;

    [Header("UI Raycasting")]
    public Canvas pointerCanvas;
    public EventSystem eventSys;           

    [Header("Pinch Detection (Strength)")]
    [Range(0f, 1f)] public float pinchOnThreshold = 0.9f;
    [Range(0f, 1f)] public float pinchOffThreshold = 0.7f;

    [Header("Click 'Forgiveness'")]
    [Tooltip("El 'grosor' del rayo en unidades de Unity. Un valor más alto es más fácil de clickar.")]
    public float clickRadius = 0.5f;

    private bool isPinching = false;
    private bool eventSystemChecked = false;

    void Start()
    {
        if (leapProvider == null) Debug.LogError("Asigna el LeapServiceProvider en el Inspector.");
        if (mainCamera == null) mainCamera = Camera.main;

        // EventSystem
        if (EventSystem.current == null)
        {
            Debug.LogError("¡FALTA UN EVENTSYSTEM! Añade uno desde 'UI > Event System'.");
            eventSystemChecked = false;
        }
        else
        {
            eventSystemChecked = true;
        }
        if (eventSys == null) eventSys = EventSystem.current;
    }

    //----------------------------------------------------//
    //Este método lee el CurrentFrame del LeapServiceProvider
    //y toma solo la mano derecha. A partir de PinchStrength
    //y con dos umbrales decide cual de las dos siguientes
    //acciones realizar:
    //
    //pinchOnThreshold, transición a estado “pinch activo”
    //(inicio de click). Cuando se detecta el inicio del pinch,
    //llama a TrySelectPlanet
    //
    //pinchOffThreshold, salida del estado “pinch”.
    //----------------------------------------------------//
    void Update()
    {
        Frame frame = leapProvider.CurrentFrame;
        if (frame == null || frame.Hands.Count == 0)
        {
            if (isPinching) isPinching = false;
            return;
        }

        //SOLO MANO DERECHA
        Hand hand = frame.Hands.Find(h => h.IsRight);
        if (hand == null)
        {
            // No hay mano derecha visible, no hacemos nada
            if (isPinching) isPinching = false;
            return;
        }

        float currentPinchStrength = hand.PinchStrength;

        if (currentPinchStrength > pinchOnThreshold && !isPinching)
        {
            isPinching = true;
            TrySelectPlanet();
        }
        else if (currentPinchStrength < pinchOffThreshold && isPinching)
        {
            isPinching = false;
        }
    }

    //---------------------------------------------------------------------------------//
    //Este método primero lanza un raycast de UI en la posición del puntero Leap y,
    //si encuentra un botón u otro elemento clicable, simula un click completo sobre él.
    //Si no hay UI clicada, hace un SphereCast en 3D desde la cámara y, si golpea un
    //PlanetClickable, ejecuta su OnPointerClick(selecciona el planeta).
    //---------------------------------------------------------------------------------//
    public void TrySelectPlanet()
    {
        if (!eventSystemChecked) return;
        if (mainCamera == null || pointerUI == null) return;

        Vector3 screenPos = pointerUI.position;

        // ---------- 1) RAYCAST DE UI (TODOS LOS CANVAS) ----------
        var ped = new PointerEventData(EventSystem.current)
        {
            position = screenPos,
            button = PointerEventData.InputButton.Left,
            clickCount = 1
        };

        var results = new List<RaycastResult>();
        EventSystem.current.RaycastAll(ped, results); // consulta a TODOS los BaseRaycasters

        for (int i = 0; i < results.Count; i++)
        {
            var rr = results[i];

            // Ignorar elementos del pointerCanvas (si se ha asignado)
            if (pointerCanvas != null && rr.gameObject != null)
            {
                if (rr.gameObject.transform.IsChildOf(pointerCanvas.transform))
                    continue;
            }

            // Sube al primer ancestro que implemente IPointerClickHandler (p.ej. Button)
            GameObject handlerGo = ExecuteEvents.GetEventHandler<IPointerClickHandler>(rr.gameObject);
            if (handlerGo != null)
            {
                // (Opcional) simular down/up para plena compatibilidad con Selectable
                ExecuteEvents.Execute(handlerGo, ped, ExecuteEvents.pointerDownHandler);
                ExecuteEvents.Execute(handlerGo, ped, ExecuteEvents.pointerUpHandler);
                ExecuteEvents.Execute(handlerGo, ped, ExecuteEvents.pointerClickHandler);

                Debug.Log($"Pinch UI → Click en {handlerGo.name} (hit: {rr.gameObject.name})");
                return; // ya clicamos UI; no seguimos con 3D
            }
        }

        // ---------- 2) FÍSICAS 3D (planetas) ----------
        Ray ray = mainCamera.ScreenPointToRay(screenPos);
        if (Physics.SphereCast(ray, clickRadius, out RaycastHit hit, 1000f))
        {
            var clickable = hit.collider.GetComponent<PlanetClickable>();
            if (clickable != null)
            {
                var eventData = new PointerEventData(EventSystem.current);
                clickable.OnPointerClick(eventData);
                Debug.Log($"Pinch sobre {hit.collider.name} → Click ejecutado.");
            }
            else
            {
                Debug.Log($"Pinch sobre {hit.collider.name}, pero no tiene 'PlanetClickable'.");
            }
        }
        else
        {
            Debug.Log("Pinch en el aire, no se golpeó nada.");
        }
    }
}
