/*****************************************************/
//Este eumerado define los estados globales en los que
//puede estar la aplicación del planetario. Este enum se
//usa en el GameManager para controlar y decidir qué UI y
//comportamientos están activos en cada momento:
//
//StandBy: pantalla de espera inicial, antes de detectar
//manos o primer click).
//
//MapsPanel: estado asociado al panel de mapas del museo.
//
//MainPanel: menú principal visible.
//
//MainView: vista general de la escena 3D de planetas,
//sin estar haciendo zoom a ninguno en concreto.
//
//ZoomView: estado en el que la cámara está centrada en un
//planeta concreto (zoom in) y se muestra su panel de información.
/*****************************************************/
public enum GameStates
{
    StandBy, MapsPanel, MainPanel, MainView, ZoomView    
}
