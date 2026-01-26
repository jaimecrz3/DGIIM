using Codice.CM.Common;
using NUnit.Framework;
using NUnit.Framework.Internal;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Numerics;
using TMPro;
using UnityEditor;
using UnityEditor.Experimental.GraphView;
using UnityEngine;
using UnityEngine.UI;
//using static System.Net.Mime.MediaTypeNames;
using static UnityEditor.PlayerSettings;
using static UnityEngine.Rendering.DebugUI;
using static UnityEngine.Rendering.VolumeComponent;


/*****************************************************************/
//Esta clase se encarga de gestionar toda la UI del planetario. Es
//un singleton (UIManager.Instance) al que acceden otros sistemas.
//Gestiona qué paneles están visibles (stand-by, menú principal,
//panel de planeta, mapas, banner de modo, tutoriales) y cómo
//aparecen/desaparecen usando efectos de fade. Actualiza los textos
//multi-idioma usando PlanetTextCSVLoader y reacciona cuando cambia el
//idioma. Ademas coordina la UI con el GameManager.
/*****************************************************************/
public class UIManager : MonoBehaviour
{
    // --- Singleton ---
    public static UIManager Instance { get; private set; }

    [Header("Fade Settings")]
    [Tooltip("Duración por defecto del fade in/out")]
    public float defaultFadeDuration = 0.35f;
    public AnimationCurve fadeEase = AnimationCurve.EaseInOut(0, 0, 1, 1);

    // Control interno: una corrutina por panel para no solapar fades
    private readonly Dictionary<GameObject, Coroutine> _runningFades = new();


    [Header("------ Start Menu ------")]
    public GameObject startMenuCanvas;
    public TMP_Text startButtonText;
    public TMP_Text GoToMapsButtonText;
    public TMP_Text GoTo3DMapsButtonText;
    public TMP_Text planetariumTitleText;

    [Header("------ Planet Elements ------")]
    public GameObject PlanetMenu;

    public TMP_Text planetTitle;
    public TMP_Text planetDescription;


    [Header("------ Mode Banner ------")]
    public GameObject modeBannerPanel;   // Panel raíz con CanvasGroup
    public TMP_Text modeButtonText;

    [Tooltip("Contenido visual de cada modo dentro del panel")]
    public GameObject kidContent;
    public GameObject normalContent;
    public GameObject expertContent;

    [Header("Mode Banner Behavior")]
    public float modeBannerDuration = 3.5f; // cuanto tiempo se ve el banner

    [Header("Videos")]
    public GameObject tutorialsCanva;

    public GameObject standByCanva;

    private PlanetTextCSVLoader textsDB;

    [Header("------ Museum Maps ------")]                // NUEVO
    public GameObject mapsPanel;                        // PanelMapas (GameObject entero)
    public Image mapsImage;                             // Image donde se ve el plano

    [Header("Museum Maps Sprites")]                     // NUEVO
    public Sprite plantaBajaSprite;
    public Sprite primeraPlantaNiñosSprite;
    public Sprite primeraPlantaSprite;
    public Sprite segundaPlantaSprite;
    public Sprite terceraPlantaSprite;

    [Header("------ Map Panel Texts ------")]
    public TMP_Text mapsTitleText;              // "Mapas del Planetario"
    public TMP_Text backMapsButtonText;         // "Atrás"

    public TMP_Text plantaBajaButtonText;       // "Planta Baja"
    public TMP_Text primeraPlantaButtonText;    // "Primera Planta"
    public TMP_Text segundaPlantaButtonText;    // "Segunda Planta"
    public TMP_Text terceraPlantaButtonText;    // "Tercera Planta"


    public int tutorialPlaying = -1;

    //---------------------------------------------------------------//
    //Configura el patrón singleton.
    //Entrada: ninguna.
    //Salida: ninguna.Si ya hay otro UIManager, destruye este objeto.
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

    }

    //---------------------------------------------------------------//
    //Inicializa referencias, se suscribe al evento de cambio de
    //idioma(textsDB.languageChange += refreshUI) y deja los paneles en
    //su estado inicial(stand-by activo, resto oculto).
    //Entrada/Salida: ninguna.
    //---------------------------------------------------------------//
    private void Start()
    {
        textsDB = PlanetTextCSVLoader.Instance;
        textsDB.languageChange += refreshUI;

        if (standByCanva) standByCanva.SetActive(true);
        // Panel oculto desde el principio
        if (startMenuCanvas) startMenuCanvas.SetActive(false);
        if (PlanetMenu) PlanetMenu.SetActive(false);

        if (modeBannerPanel)
        {
            modeBannerPanel.SetActive(false);
            HideAllModeContents();
        }

        if (mapsPanel) mapsPanel.SetActive(false);
        if (tutorialsCanva) tutorialsCanva.SetActive(false);

    }

    //---------------------------------------------------------------//
    //Manejador del botón de “Start visit”: oculta el menú inicial con
    //fade y avisa al GameManager para iniciar la visita.
    //Entrada: ninguna(se llama desde la UI).
    //Salida: ninguna.
    //---------------------------------------------------------------//
    public void ClickedStart()
    {
        //Debug.Log("Clicked Start");
        if (startMenuCanvas) ShowPanelFade(startMenuCanvas, false);
        GameManager.Instance.StartVisit();
    }

    //---------------------------------------------------------------//
    //Oculta el menú de inicio y muestra el panel de mapas, inicializando
    //el sprite a “Planta baja”.
    //Entrada: ninguna.
    //Salida: ninguna.
    //---------------------------------------------------------------//
    public void OpenMapsPanel()
    {
        // Ocultamos menú de inicio y mostramos PanelMapas
        if (startMenuCanvas) ShowPanelFade(startMenuCanvas, false);
        if (mapsPanel) ShowPanelFade(mapsPanel, true);
        if (mapsImage && plantaBajaSprite)
            mapsImage.sprite = plantaBajaSprite;
    }

    //---------------------------------------------------------------//
    //Hace lo inverso: oculta el panel de mapas y vuelve a mostrar el
    //menú de inicio.
    //---------------------------------------------------------------//
    public void CloseMapsPanel()
    {
        // Volver al menú de inicio
        if (mapsPanel) ShowPanelFade(mapsPanel, false);
        if (startMenuCanvas) ShowPanelFade(startMenuCanvas, true);
    }

    //---------------------------------------------------------------//
    //Los siguientes métodos cambian la imagen de mapas al sprite de
    //la planta correspondiente.
    //---------------------------------------------------------------//
    public void ShowPlantaBaja()
    {
        if (mapsImage && plantaBajaSprite)
            mapsImage.sprite = plantaBajaSprite;
    }

    public void ShowPrimeraPlanta()
    {
        if(GameManager.Instance?.CurrentMode == GameMode.Kid)
        {
            if (mapsImage && primeraPlantaNiñosSprite)
                mapsImage.sprite = primeraPlantaNiñosSprite;
        }else if (mapsImage && primeraPlantaSprite)
            mapsImage.sprite = primeraPlantaSprite;
    }

    public void ShowSegundaPlanta()
    {
        if (mapsImage && segundaPlantaSprite)
            mapsImage.sprite = segundaPlantaSprite;
    }

    public void ShowTerceraPlanta()
    {
        if (mapsImage && terceraPlantaSprite)
            mapsImage.sprite = terceraPlantaSprite;
    }

    //---------------------------------------------------------------//
    //Manejador del botón de “Mapa del museo 3D”: oculta el menú y
    //delega en GameManager.GoToMuseumMap() para cambiar de escena.
    //---------------------------------------------------------------//
    public void ClickedMuseumMap()
    {
        if (startMenuCanvas) ShowPanelFade(startMenuCanvas, false);

        // delegamos la lógica al GameManager
        GameManager.Instance.GoToMuseumMap();
    }

    //---------------------------------------------------------------//
    //Vuelve a mostrar el menú principal.
    //---------------------------------------------------------------//
    public void GoToMainPanel()
    {
        if (startMenuCanvas) ShowPanelFade(startMenuCanvas, true);
    }

    //---------------------------------------------------------------//
    //Transición desde la pantalla de stand-by al menú principal, con
    //fades independientes en cada panel.
    //---------------------------------------------------------------//
    public void SalirStandBy ()
    {
        ShowPanelFade(standByCanva, false, 1f);
        ShowPanelFade(startMenuCanvas, true, 1.5f);
    }

    //---------------------------------------------------------------//
    //Método de prueba para cambiar el GameMode por ciclos
    //(Kid -> Normal -> Expert -> Kid) cuando no se usa Leap;
    //llama a GameManager.SetMode.
    //---------------------------------------------------------------//
    public void ChangeMode()
    {
        GameMode? pose = null;
        if (startMenuCanvas != null)
        {
            if (GameManager.Instance?.CurrentMode == GameMode.Kid)
            {
                pose = GameMode.Normal;
            }
            else if (GameManager.Instance?.CurrentMode == GameMode.Normal)
            {
                pose = GameMode.Expert;
            }
            else
            {
                pose = GameMode.Kid;
            }
            GameManager.Instance?.SetMode(pose.Value);
        }

    }

    //---------------------------------------------------------------//
    //Método genérico para mostrar u ocultar un panel con fade usando CanvasGroup.
    //Entrada:
    //panel: panel a animar.
    //show: true = fade in, false = fade out.
    //duration: duración del fade(por defecto defaultFadeDuration).
    //setInteractable: si debe activar/desactivar interactable.
    //setBlocksRaycasts: si debe activar/desactivar blocksRaycasts.
    //
    //Salida: ninguna (lanza una corrutina interna que hace la animación).
    //---------------------------------------------------------------//
    public void ShowPanelFade(GameObject panel, bool show = true, float? duration = null, bool setInteractable = true, bool setBlocksRaycasts = true)
    {
        if (panel == null) return;

        // Si vamos a MOSTRAR y el panel está inactivo, creamos/aseguramos el CanvasGroup con alpha=0 antes de activarlo
        CanvasGroup cg = (!panel.activeSelf && show)
            ? EnsureCanvasGroup(panel, initialAlpha: 0f)   // clave: invisible antes de SetActive(true)
            : EnsureCanvasGroup(panel);                    // reutiliza el existente sin forzar alpha

        if (show && !panel.activeSelf)
            panel.SetActive(true); // ya está con alpha=0, no habrá "pop"

        float dur = duration ?? defaultFadeDuration;

        // Evita solapar fades previos en este panel
        if (_runningFades.TryGetValue(panel, out var co) && co != null)
            StopCoroutine(co);

        // Si hacemos fade IN, habilita input durante la transición 
        if (show)
        {
            if (setInteractable) cg.interactable = true;
            if (setBlocksRaycasts) cg.blocksRaycasts = true;
        }

        _runningFades[panel] = StartCoroutine(FadeCanvasGroup(
            panel, cg,
            target: show ? 1f : 0f,
            duration: dur,
            setInteractable: setInteractable,
            setBlocksRaycasts: setBlocksRaycasts
        ));
    }

    private IEnumerator FadeCanvasGroup(GameObject panel, CanvasGroup cg, float target, float duration, bool setInteractable, bool setBlocksRaycasts)
    {
        float start = cg.alpha;
        float t = 0f;
        if (target > start)
        {
            if (setInteractable) cg.interactable = true;
            if (setBlocksRaycasts) cg.blocksRaycasts = true;
        }

        if (duration <= 0f)
        {
            cg.alpha = target;
        }
        else
        {
            while (t < duration)
            {
                t += Time.unscaledDeltaTime; 
                float k = fadeEase.Evaluate(Mathf.Clamp01(t / duration));
                cg.alpha = Mathf.Lerp(start, target, k);
                yield return null;
            }
            cg.alpha = target;
        }

        // Si hemos hecho fade out completo, desactivar interacción y el propio panel
        if (Mathf.Approximately(target, 0f))
        {
            if (setInteractable) cg.interactable = false;
            if (setBlocksRaycasts) cg.blocksRaycasts = false;
            panel.SetActive(false);
        }

        _runningFades[panel] = null;
    }

    private CanvasGroup EnsureCanvasGroup(GameObject panel, float? initialAlpha = null)
    {
        var cg = panel.GetComponent<CanvasGroup>();
        if (cg == null) cg = panel.AddComponent<CanvasGroup>();
        if (initialAlpha.HasValue) cg.alpha = initialAlpha.Value;
        return cg;
    }

    //---------------------------------------------------------------//
    //Muestra u oculta el panel de información del planeta actual.
    //Si show es true, refresca título y descripción según el planeta
    //objetivo y el modo de juego.
    //Entrada: show(true/false).
    //Salida: ninguna.
    //---------------------------------------------------------------//
    public void ShowPlanetPanel(bool show = true)
    {
        if (!PlanetMenu)
        {
            return;
        }

        if (show == true)
        {
            PlanetClickable p = GameManager.Instance.GetCurrentTarget();
            if (p != null)
            {

            }
            else
            {
                Debug.Log("Error, planeta solicitado pero no hay ninguno como objetivo");
            }

            SetPlanetTitle(p);
            SetPlanetInfo(p, GameManager.Instance.CurrentMode);
        }

        ShowPanelFade(PlanetMenu, show);
    }

    //---------------------------------------------------------------//
    //Escribe en planetTitle el nombre del planeta.
    //---------------------------------------------------------------//
    public void SetPlanetTitle(PlanetClickable planet)
    {
        if (planetTitle) planetTitle.text = textsDB.GetNombre(planet);
    }

    //---------------------------------------------------------------//
    //Rellena la descripción (planetDescription) con el texto
    //correspondiente al planeta y al modo (Kid/Normal/Expert)
    //---------------------------------------------------------------//
    public void SetPlanetInfo(PlanetClickable planet, GameMode mode)
    {
        if (planetDescription)
        {
            string info = textsDB.GetInfo(planet, mode);

            planetDescription.text = info;
        }
    }

    //---------------------------------------------------------------//
    //Intenta convertir newLanguage a Language(TryParseLanguage),
    //cambia el idioma actual en PlanetTextCSVLoader y llama a refreshUI().
    //Entrada: código de idioma como string ("ES", "EN", etc.).
    //Salida: ninguna.
    //---------------------------------------------------------------//
    public void changeLanguage(string newLanguage)
    {
        Language lan;

        if (PlanetTextCSVLoader.TryParseLanguage(newLanguage, out lan))
        {
            PlanetTextCSVLoader.Instance.setLanguage(lan);
            refreshUI();
        }
    }

    //---------------------------------------------------------------//
    //Actualiza textos de la UI según el estado global del juego:
    //En MainPanel: textos de botones del menú y título del planetario.
    //En MapsPanel: textos del panel de mapas(título, botón atrás, nombres de plantas).
    //En vista de planetas: título y descripción del planeta objetivo.
    //---------------------------------------------------------------//
    public void refreshUI()
    {
        if (GameManager.Instance.GetState() == GameStates.MainPanel)
        {
            startButtonText.text = PlanetTextCSVLoader.Instance.GetText("start_button");
            GoToMapsButtonText.text = PlanetTextCSVLoader.Instance.GetText("maps_button");
            GoTo3DMapsButtonText.text = PlanetTextCSVLoader.Instance.GetText("3Dmaps_button");
            modeButtonText.text = PlanetTextCSVLoader.Instance.GetText("mode_button");

            if (planetariumTitleText)
                planetariumTitleText.text = PlanetTextCSVLoader.Instance.GetText("planetarium_name");
            return;
        }

        if (GameManager.Instance.GetState() == GameStates.MapsPanel)
        {
            //Debug.Log("REFRESH EN PANEL");
            if (mapsTitleText)
                mapsTitleText.text = textsDB.GetText("maps_title");

            if (backMapsButtonText)
                backMapsButtonText.text = textsDB.GetText("maps_back");

            if (plantaBajaButtonText)
                plantaBajaButtonText.text = textsDB.GetText("maps_ground");

            if (primeraPlantaButtonText)
                primeraPlantaButtonText.text = textsDB.GetText("maps_first");

            if (segundaPlantaButtonText)
                segundaPlantaButtonText.text = textsDB.GetText("maps_second");

            if (terceraPlantaButtonText)
                terceraPlantaButtonText.text = textsDB.GetText("maps_third");
            return;
        }

        if (GameManager.Instance.GetCurrentTarget() == null) return;

        // Si hay un target, actualizamos los textos
        PlanetClickable p = GameManager.Instance.GetCurrentTarget();
        SetPlanetInfo(p, GameManager.Instance.CurrentMode);

        SetPlanetTitle(p);
        //BuildPlanetPages(p.GetId());


    }

    //---------------------------------------------------------------//
    //Desactiva los GameObjects kidContent, normalContent y expertContent.
    //---------------------------------------------------------------//
    private void HideAllModeContents()
    {
        if (kidContent) kidContent.SetActive(false);
        if (normalContent) normalContent.SetActive(false);
        if (expertContent) expertContent.SetActive(false);
    }

    //---------------------------------------------------------------//
    //Muestra el banner de modo: desactiva todos los contenidos,
    //activa solo el correspondiente al mode, hace fade in del panel y programa un fade out tras modeBannerDuration.
    //Entrada: modo actual(GameMode).
    //Salida: ninguna.
    //---------------------------------------------------------------//
    public void ShowModeBanner(GameMode mode)
    {
        if (!modeBannerPanel) return;

        // Apagar todos los hijos primero
        HideAllModeContents();

        // Encender solo el que toca
        GameObject toActivate = null;
        switch (mode)
        {
            case GameMode.Kid:
                toActivate = kidContent;
                break;
            case GameMode.Normal:
                toActivate = normalContent;
                break;
            case GameMode.Expert:
                toActivate = expertContent;
                break;
        }

        if (toActivate) toActivate.SetActive(true);

        // Mostrar el panel con tu sistema de fade
        ShowPanelFade(modeBannerPanel, true);

        // Opcional: ocultarlo solo después de unos segundos
        StartCoroutine(HideModeBannerAfterDelay(modeBannerDuration));
    }

    private IEnumerator HideModeBannerAfterDelay(float delay)
    {
        yield return new WaitForSecondsRealtime(delay);
        ShowPanelFade(modeBannerPanel, false);
    }

    //---------------------------------------------------------------//
    //Muestra el canvas de tutoriales, llama a VideoManager.PlayTutorial(id)
    //y guarda qué tutorial está activo en tutorialPlaying.
    //Entrada: id del tutorial(0 = click, 1 = swipe, 2 = swipe back).
    //Salida: ninguna.
    //---------------------------------------------------------------//

    public void ShowTutorial (int id = 0)
    {
        VideoManager.Instance.PlayTutorial(id);
        tutorialPlaying = id;
        ShowPanelFade(tutorialsCanva, true);
    }

    //---------------------------------------------------------------//
    //Oculta el canvas de tutoriales con fade, pone tutorialPlaying a -1
    //y detiene el vídeo actual
    //---------------------------------------------------------------//
    public void HideTutorial()
    {
        tutorialPlaying = -1;
        ShowPanelFade(tutorialsCanva, false);
        VideoManager.Instance.StopTutorial();
    }

}
