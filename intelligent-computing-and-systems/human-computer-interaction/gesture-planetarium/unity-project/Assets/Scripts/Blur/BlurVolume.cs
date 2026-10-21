using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;
using System.Collections;

//*************************************************************//
//Esta clase controla el efecto de desenfoque de fondo,  usado
//por el GameManager para enfatizar u ocultar el entorno según
//el estado de la visita.
//*************************************************************//
public class BlurVolume : MonoBehaviour
{
    [Header("Parámetros existentes")]
    public float focusDistance = 0.75f; // valor ON

    [Header("Transición suave (sin weight)")]
    public float focusDistanceOff = 10f;         // valor OFF (lejos)
    public float fadeDuration = 0.5f;            // segundos
    public AnimationCurve ease =                 
        AnimationCurve.EaseInOut(0, 0, 1, 1);

    private DepthOfField dof;
    private bool toggleDof = true;               // INICIO: ACTIVO
    private Coroutine fadeCo;

    void Start()
    {
        var vol = GetComponent<Volume>();
        if (vol == null || vol.profile == null || !vol.profile.TryGet(out dof))
        {
            Debug.LogError("Volume sin Depth Of Field");
            enabled = false;
            return;
        }

        // Estado inicial ACTIVO
        dof.active = true;
        dof.focusDistance.value = focusDistance;
    }

    public void ToggleBackgroundBlur()
    {
        toggleDof = !toggleDof;

        // Cancela transición previa si la hubiera
        if (fadeCo != null) StopCoroutine(fadeCo);
        fadeCo = StartCoroutine(FadeFocusDistance(
            dof.focusDistance.value,
            toggleDof ? focusDistance : focusDistanceOff,
            fadeDuration,
            toggleDof
        ));
    }

    private IEnumerator FadeFocusDistance(float start, float end, float duration, bool finalActive)
    {
        // Asegura que el efecto esté activo durante la transición
        dof.active = true;

        if (duration <= 0f)
        {
            dof.focusDistance.value = end;
            dof.active = finalActive;
            yield break;
        }

        float t = 0f;
        while (t < duration)
        {
            t += Time.deltaTime;
            float k = ease.Evaluate(Mathf.Clamp01(t / duration));
            dof.focusDistance.value = Mathf.Lerp(start, end, k);
            yield return null;
        }

        dof.focusDistance.value = end;
        dof.active = finalActive; // si quedó en OFF, ahorra coste
    }
}
