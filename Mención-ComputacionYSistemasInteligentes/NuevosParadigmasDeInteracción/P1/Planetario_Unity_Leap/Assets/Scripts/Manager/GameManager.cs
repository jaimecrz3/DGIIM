using System.Collections;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.SceneManagement;

public enum GameMode { Kid, Normal, Expert }

/*****************************************************************/
//Esta clase se encarga de gestionar el estado global del planetario.
//Es un singleton (GameManager.Instance) que coordina el modo de juego
//(Kid/Normal/Expert), el flujo de pantallas (StandBy, menú, mapas,
//vista principal, zoom a planeta), las transiciones de cámara y blur,
//los tutoriales y los cambios de escena (por ejemplo, al museo).
/*****************************************************************/
public class GameManager : MonoBehaviour
{
    // --- Singleton ---
    public static GameManager Instance { get; private set; }
    public GameMode CurrentMode { get; private set; } = GameMode.Normal;

    // Fields
    private UIManager uiManager;
    private PlanetTextCSVLoader textsDB;

    [SerializeField] private BlurVolume sceneVolume;
    [SerializeField] private PlanetZoomController cam;

    private GameStates _state;

    [Header("Museum Scene")]
    [SerializeField] private string museumSceneName = "MuseoMapa";

    // TUTORIALES
    private bool _showClickTutorial = true;
    private bool _showSwipeTutorial = true;
    private bool _showSwipeBackTutorial = true;

    //---------------------------------------------------------------//
    //Configura el patrón singleton y se suscribe al evento
    //OnZoomCompleted de la cámara para reaccionar al final de un zoom.
    //Entrada: ninguna.
    //Salida: ninguna. Si ya existe otro GameManager, destruye este.
    //---------------------------------------------------------------//
    void Awake()
    {
        // Configura el Singleton
        if (Instance != null && Instance != this)
        {
            Destroy(gameObject);
        }
        else
        {
            Instance = this;
        }

        if (cam != null)
        {
            cam.OnZoomCompleted -= HandleZoomCompleted; // evita doble suscripción
            cam.OnZoomCompleted += HandleZoomCompleted;
        }
    }

    //---------------------------------------------------------------//
    //Mientras el estado global sea StandBy, escucha un click de ratón
    //para salir de la pantalla de espera y pasar al menú principal.
    //Entrada/Salida: ninguna.
    //---------------------------------------------------------------//
    void Update()
    {
        // Solo reaccionamos a input mientras estamos en StandBy
        if (_state != GameStates.StandBy) return;

        // Click de ratón 
        if (Input.GetMouseButtonDown(0))
        {
            SalirStandBy();
            return;
        }
    }

    //---------------------------------------------------------------//
    //Devuelve el planeta actualmente seleccionado por la cámara
    //(currentTarget del PlanetZoomController).
    //Entrada: ninguna.
    //Salida: referencia a PlanetClickable actual (puede ser null).
    //---------------------------------------------------------------//
    public PlanetClickable GetCurrentTarget()
    {
        return cam.currentTarget;
    }

    //---------------------------------------------------------------//
    //Devuelve el estado global actual del juego (StandBy, MainPanel,
    //MapsPanel, MainView, ZoomView).
    //Entrada: ninguna.
    //Salida: valor del enum GameStates.
    //---------------------------------------------------------------//
    public GameStates GetState()
    {
        return _state;
    }

    //---------------------------------------------------------------//
    //Cambia el modo de juego (Kid, Normal, Expert) si el nuevo modo
    //es distinto al actual. Muestra el banner de modo y fuerza a la UI
    //a refrescar textos y contenidos asociados al modo.
    //Entrada: mode (nuevo GameMode).
    //Salida: ninguna.
    //---------------------------------------------------------------//
    public void SetMode(GameMode mode)
    {
        if (CurrentMode == mode) return;
        CurrentMode = mode;
        Debug.Log($"Modo cambiado a: {mode}");

        UIManager.Instance?.ShowModeBanner(mode);
        uiManager.refreshUI();
        // Aquí se podría activar lógica específica de dificultad, etc.
    }

    //---------------------------------------------------------------//
    //Inicializa referencias a UIManager y PlanetTextCSVLoader,
    //pone el estado inicial en StandBy y refresca la UI.
    //Entrada/Salida: ninguna.
    //---------------------------------------------------------------//
    private void Start()
    {
        uiManager = UIManager.Instance;
        textsDB = PlanetTextCSVLoader.Instance;

        _state = GameStates.StandBy;

        uiManager.refreshUI();
    }

    //---------------------------------------------------------------//
    //Transición desde el estado StandBy al menú principal:
    //lanza la animación de salida del stand-by en la UI, actualiza
    //el estado a MainPanel y refresca textos.
    //Entrada/Salida: ninguna.
    //---------------------------------------------------------------//
    public void SalirStandBy()
    {
        // Si ya hemos salido, no hacemos nada
        if (_state != GameStates.StandBy) return;

        uiManager.SalirStandBy();
        _state = GameStates.MainPanel;
        uiManager.refreshUI();
    }

    //---------------------------------------------------------------//
    //Comienza la visita al planetario: invierte el blur de fondo,
    //cambia el estado a MainView y, si procede, muestra el tutorial
    //inicial de "click" (id 0).
    //Entrada/Salida: ninguna.
    //---------------------------------------------------------------//
    public void StartVisit()
    {
        ToggleBackgroundBlur();
        _state = GameStates.MainView;

        if (_showClickTutorial)
        {
            Debug.Log("Show Click");
            uiManager.ShowTutorial(0);
        }
    }

    //---------------------------------------------------------------//
    //Vuelve al panel principal desde la vista 3D: alterna el blur,
    //cambia el estado a MainPanel, muestra el menú y refresca la UI.
    //Entrada/Salida: ninguna.
    //---------------------------------------------------------------//
    public void GoToMainPanel()
    {
        ToggleBackgroundBlur();
        _state = GameStates.MainPanel;

        uiManager.GoToMainPanel();
        uiManager.refreshUI();
    }

    //---------------------------------------------------------------//
    //Carga la escena del museo (Mapa 3D) si se ha configurado un
    //nombre de escena válido.
    //Entrada/Salida: ninguna.
    //---------------------------------------------------------------//
    public void GoToMuseumMap()
    {
        if (string.IsNullOrEmpty(museumSceneName))
        {
            Debug.LogError("GameManager: museumSceneName no está configurado.");
            return;
        }

        SceneManager.LoadScene(museumSceneName);
    }

    //---------------------------------------------------------------//
    //Pide al componente BlurVolume que alterne el estado del desenfoque
    //de fondo (toggle del Depth of Field).
    //Entrada/Salida: ninguna.
    //---------------------------------------------------------------//
    public void ToggleBackgroundBlur()
    {
        sceneVolume.ToggleBackgroundBlur();
    }

    //---------------------------------------------------------------//
    //Gestiona la petición de zoom sobre un planeta:
    //- Si el tutorial de click sigue activo, lo marca como visto y lo
    //  oculta si estaba en reproducción.
    //- Si el estado actual es MainPanel, ignora la petición.
    //- Llama a PlanetZoomController.RequestZoom(planet) y, según
    //  el resultado:
    //     false  -> zoom out (vuelta a MainView, oculta panel planeta).
    //     true   -> zoom in (pasa a ZoomView, la UI se actualizará
    //               en HandleZoomCompleted cuando termine la animación).
    //Entrada: planet (PlanetClickable a hacer zoom).
    //Salida: ninguna.
    //---------------------------------------------------------------//
    public void RequestZoom(PlanetClickable planet)
    {
        if (_showClickTutorial)
        {
            _showClickTutorial = false;

            if (uiManager.tutorialPlaying == 0)
            {
                uiManager.HideTutorial();
            }
        }

        if (_state == GameStates.MainPanel)
        {
            return;
        }

        bool? zoomIn = cam.RequestZoom(planet);

        if (zoomIn == false)
        {
            // Zoom out: ocultar panel directamente
            _state = GameStates.MainView;
            uiManager.ShowPlanetPanel(false);
        }
        else
        {
            // Zoom in: cuando termine el zoom, HandleZoomCompleted se encargará
            // de mostrar panel y refrescar
            _state = GameStates.ZoomView;
        }
    }

    //---------------------------------------------------------------//
    //Callback llamado al terminar una transición de zoom de cámara.
    //Si zoomIn es true, muestra el panel del planeta que ha quedado
    //como currentTarget, refresca la UI y lanza el tutorial de
    //“volver atrás” (id 2) o de “swipe lateral” (id 1) según flags.
    //Entrada: zoomIn (true = zoom in completado, false = zoom out).
    //Salida: ninguna.
    //---------------------------------------------------------------//
    private void HandleZoomCompleted(bool zoomIn)
    {
        if (zoomIn)
        {
            // Mostrar panel del planeta ACTUAL que está como target en la cámara
            var planet = cam.currentTarget;
            if (planet != null)
            {
                uiManager.ShowPlanetPanel(true);
                uiManager.refreshUI();

                if (_showSwipeBackTutorial)
                {
                    uiManager.ShowTutorial(2);
                    return;
                }

                if (_showSwipeTutorial) uiManager.ShowTutorial(1);
            }
        }
    }

    //---------------------------------------------------------------//
    //Manejador del gesto de swipe hacia la derecha (mano). Selecciona
    //el planeta vecino siguiente en la lista con wrap-around, actualiza
    //la UI si no estamos en MainPanel y, si procede, oculta el tutorial
    //de swipe lateral.
    //Entrada: _(Leap.Hand) mano que ha generado el gesto.
    //Salida: ninguna.
    //---------------------------------------------------------------//
    public void HandleSwipeRight(Leap.Hand _)
    {
        if (cam && cam.currentTarget)
        {
            cam.SelectNeighbor(+1, wrap: true);
            if (_state != GameStates.MainPanel)
            {
                uiManager.refreshUI();    // planeta nuevo → textos nuevos
            }

            _showSwipeTutorial = false;
            if (uiManager.tutorialPlaying == 1)
            {
                uiManager.HideTutorial();
            }
        }
    }

    //---------------------------------------------------------------//
    //Manejador del gesto de swipe hacia la izquierda. Funciona como
    //HandleSwipeRight pero moviéndose al planeta anterior.
    //Entrada: _(Leap.Hand) mano que ha generado el gesto .
    //Salida: ninguna.
    //---------------------------------------------------------------//
    public void HandleSwipeLeft(Leap.Hand _)
    {
        if (cam && cam.currentTarget)
        {
            cam.SelectNeighbor(-1, wrap: true);
            if (_state != GameStates.MainPanel)
            {
                uiManager.refreshUI();
            }

            _showSwipeTutorial = false;
            if (uiManager.tutorialPlaying == 1)
            {
                uiManager.HideTutorial();
            }
        }
    }

    //---------------------------------------------------------------//
    //Manejador del gesto de swipe “hacia atrás” (en profundidad).
    //Simplemente delega en GoBack() para resolver hacia dónde volver.
    //Entrada: _(Leap.Hand) mano que ha generado el gesto.
    //Salida: ninguna.
    //---------------------------------------------------------------//
    public void HandleSwipeBack(Leap.Hand _)
    {
        GoBack();
    }

    //---------------------------------------------------------------//
    //Centraliza la lógica de “volver atrás” en el flujo:
    //- Si estamos en MainPanel, no hace nada.
    //- Desactiva la flag de tutorial de swipe back y oculta cualquier
    //  tutorial activo.
    //- Si estamos en MapsPanel, cierra el panel de mapas.
    //- Si estamos en MainView, vuelve al MainPanel.
    //- Si estamos en ZoomView, llama a RequestZoom sobre el planeta
    //  actual para hacer zoom out.
    //Entrada/Salida: ninguna.
    //---------------------------------------------------------------//
    private void GoBack()
    {
        if (_state == GameStates.MainPanel)
        {
            return;
        }

        _showSwipeBackTutorial = false;
        if (uiManager.tutorialPlaying >= 1)
        {
            uiManager.HideTutorial();
        }

        if (_state == GameStates.MapsPanel)
        {
            CloseMapsPanel();
            return;
        }

        if (_state == GameStates.MainView)
        {
            GoToMainPanel();
        }

        if (_state == GameStates.ZoomView)
        {
            this.RequestZoom(GetCurrentTarget());
        }
    }

    //---------------------------------------------------------------//
    //Reinicia la escena actual cargándola de nuevo por índice de build.
    //Usado, por ejemplo, en el auto-reset por inactividad.
    //Entrada/Salida: ninguna.
    //---------------------------------------------------------------//
    public void ResetScene()
    {
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
    }

    //---------------------------------------------------------------//
    //Abre el panel de mapas desde el GameManager:
    //- Actualiza el estado a MapsPanel.
    //- Si corresponde, muestra el tutorial de swipe back (id 2).
    //- Pide al UIManager que abra el panel de mapas y refresca textos.
    //Entrada/Salida: ninguna.
    //---------------------------------------------------------------//
    public void OpenMapsPanel()
    {
        _state = GameStates.MapsPanel;

        if (_showSwipeBackTutorial)
        {
            uiManager.ShowTutorial(2);
        }

        uiManager.OpenMapsPanel();
        uiManager.refreshUI();
    }

    //---------------------------------------------------------------//
    //Cierra el panel de mapas y vuelve al MainPanel:
    //- Actualiza el estado a MainPanel.
    //- Oculta cualquier tutorial activo relacionado.
    //- Pide al UIManager que cierre el panel de mapas y refresca la UI.
    //Entrada/Salida: ninguna.
    //---------------------------------------------------------------//
    public void CloseMapsPanel()
    {
        _state = GameStates.MainPanel;

        if (uiManager.tutorialPlaying >= 1)
        {
            uiManager.HideTutorial();
        }

        uiManager.CloseMapsPanel();
        uiManager.refreshUI();
    }
}