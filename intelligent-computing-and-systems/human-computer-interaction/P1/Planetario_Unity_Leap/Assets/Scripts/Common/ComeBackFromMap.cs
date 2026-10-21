using UnityEditor.Graphs;
using UnityEngine;
using UnityEngine.SceneManagement;

/**********************************************************/
//Esta clase permite la navegación entre escenas. Expone el
//nombre de la escena principal y ofrece el método público
//LoadMainScene(), pensado para vincularse a un botón u otro
//evento, carga de nuevo la escena principal del planetario.
/**********************************************************/
public class BackToMainScene : MonoBehaviour
{
    // Pon aquí el nombre de la escena inicial
    public string mainSceneName = "EscenaPlanetas";

    public void LoadMainScene()
    {
        SceneManager.LoadScene(mainSceneName);
    }
}

