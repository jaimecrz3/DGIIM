using TMPro;
using UnityEngine;

/****************************************************************************/
//Esta clase gestiona los textos de la UI de la escena del plano 3D usando la
//base de datos de textos PlanetTextCSVLoader.
//Actualiza el título y el botón de “Volver” según el idioma activo,
//permitiendo también cambiar el idioma y refrescar los textos al vuelo.
/****************************************************************************/

public class Plano3DUI : MonoBehaviour
{
    public TMP_Text titleText;        // Título del plano 3D
    public TMP_Text backButtonText;   // Texto del botón "Volver"

    private PlanetTextCSVLoader textsDB;

    private void Start()
    {
        textsDB = PlanetTextCSVLoader.Instance;
        RefreshUI();
    }

    public void ChangeLanguage(string newLanguage)
    {
        Language lan;
        if (PlanetTextCSVLoader.TryParseLanguage(newLanguage, out lan))
        {
            PlanetTextCSVLoader.Instance.setLanguage(lan);
            RefreshUI();
        }
    }

    public void RefreshUI()
    {
        if (textsDB == null) textsDB = PlanetTextCSVLoader.Instance;

        if (titleText)
            titleText.text = textsDB.GetText("map3d_title");        

        if (backButtonText)
            backButtonText.text = textsDB.GetText("map3d_back");    
    }
}

