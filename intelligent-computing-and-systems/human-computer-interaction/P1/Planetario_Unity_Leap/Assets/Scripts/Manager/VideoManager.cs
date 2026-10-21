using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Video;

/*****************************************************************/
//Esta clase gestiona los vídeos de tutorial mediante un patrón
//Singleton, controlando uno o varios VideoPlayer.
//Permite reproducir clips según idioma y tipo de gesto (click,
//swipe derecha/izquierda/atrás), incluyendo un modo bucle guiado.
//Lleva la cuenta de los loops por reproductor para activar o
//mutear el audio solo en determinados ciclos y poder detener
//el tutorial cuando se desee.
/*****************************************************************/

public class VideoManager : MonoBehaviour
{
    // --- Singleton ---
    public static VideoManager Instance { get; private set; }

    [Header("VideoPlayers")]
    public VideoPlayer playerArribaIzq;
    public VideoPlayer playerMitad;

    [Header("Videos")]
    public VideoClip[] click;
    public VideoClip[] swipe_der;
    public VideoClip[] swipe_izq;
    public VideoClip[] swipe_back;

    private bool stopTutorial = true;

    // Configuración
    private const int LOOPS_PARA_SONIDO = 2; // Suena en el loop 10, 20, 30...

    // Diccionario para llevar la cuenta de loops de cada reproductor independientemente
    private Dictionary<VideoPlayer, int> loopCounters = new Dictionary<VideoPlayer, int>();

    void Awake()
    {
        if (Instance != null && Instance != this)
        {
            Destroy(gameObject);
        }
        else
        {
            Instance = this;
        }
    }

    void Start()
    {
        // Inicializamos y configuramos ambos reproductores
        ConfigurarPlayer(playerArribaIzq);
    }

    public void StopTutorial()
    {
        stopTutorial = true;
        Stop();
    }

    public void PlayTutorial(int id)
    {
        stopTutorial = false;

        int lang = (int) PlanetTextCSVLoader.Instance.currentLanguage;
        switch (id) {
            case 0:
                Play(click[lang]);
                break;
            case 1:
                StartCoroutine(SwipeTutorial());
                Debug.Log("Tutorial Swipe");
                break;
            case 2:
                Play(swipe_back[lang]);
                Debug.Log("Tutorial Swipe Back");
                break;
        }
    }

    public IEnumerator SwipeTutorial()
    {
        int lang = (int)PlanetTextCSVLoader.Instance.currentLanguage;

        while (!stopTutorial)
        {
            // --- VIDEO 1 ---
            Play(swipe_der[lang]); // Esto pone el contador a 0 internamente

            // Esperamos a que se hayan completado 3 loops
            yield return new WaitUntil(() => loopCounters[playerArribaIzq] >= 3);

            Debug.Log("Cambio a video 2");

            // --- VIDEO 2 ---
            Play(swipe_izq[lang]); // Esto pone el contador a 0 internamente otra vez

            // Esperamos a que se hayan completado 3 loops
            yield return new WaitUntil(() => loopCounters[playerArribaIzq] >= 3);

            Debug.Log("Cambio a video 1");
        }

        yield return null;
    }

    public void Play(VideoClip clip)
    {
        playerArribaIzq.Stop();

        // 1. Asignamos el clip
        playerArribaIzq.clip = clip;

        // 2. IMPORTANTE: Reseteamos el contador a 0
        if (loopCounters.ContainsKey(playerArribaIzq))
        {
            loopCounters[playerArribaIzq] = 0;
        }
        else
        {
            loopCounters.Add(playerArribaIzq, 0);
        }

        playerArribaIzq.Play();
    }

    public void Stop ()
    {
        playerArribaIzq.Stop();
    }
    private void ConfigurarPlayer(VideoPlayer vp)
    {
        if (vp == null) return;

        // Nos aseguramos de que esté en modo Loop
        vp.isLooping = true;

        // Inicializamos el contador en 0
        if (!loopCounters.ContainsKey(vp))
        {
            loopCounters.Add(vp, 0);
        }

        // Silenciamos inicialmente (para que los primeros 9 loops sean mudos)
        SetAudioMute(vp, true);

        // Nos suscribimos al evento que avisa cuando termina un loop
        vp.loopPointReached += OnLoopPointReached;
    }

    private void OnLoopPointReached(VideoPlayer vp)
    {
        if (!loopCounters.ContainsKey(vp)) return;

        // Aumentamos el contador
        loopCounters[vp]++;
        int currentLoop = loopCounters[vp];

        // LÓGICA:
        // Queremos que suene SOLAMENTE en el loop nº 10 (y múltiplos: 20, 30...).
        // El evento se dispara AL FINAL del loop.

        // Si acabamos de terminar el loop 9 (count = 9), el siguiente es el 10 -> ACTIVAMOS SONIDO
        if (currentLoop % LOOPS_PARA_SONIDO == (LOOPS_PARA_SONIDO - 1))
        {
            SetAudioMute(vp, false); // Unmute para el siguiente loop (el 10)
        }
        // Si acabamos de terminar el loop 10 (count = 10), el siguiente es el 11 -> DESACTIVAMOS SONIDO
        else
        {
            SetAudioMute(vp, true); // Mute para el resto
        }
    }

    private void SetAudioMute(VideoPlayer vp, bool mute)
    {
        // Recorremos las pistas de audio (normalmente es 1, pero por si acaso)
        for (ushort i = 0; i < vp.audioTrackCount; i++)
        {
            vp.SetDirectAudioMute(i, mute);
        }
    }

    void OnDestroy()
    {
        if (playerArribaIzq != null) playerArribaIzq.loopPointReached -= OnLoopPointReached;
        if (playerMitad != null) playerMitad.loopPointReached -= OnLoopPointReached;
    }

    void Update()
    {

    }
}