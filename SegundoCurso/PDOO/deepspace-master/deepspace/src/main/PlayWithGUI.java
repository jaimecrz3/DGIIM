/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package main;

import View.DeepSpaceView;
import View.GUI.MainWindow;
import controller.Controller;
import deepspace.GameUniverse;

/**
 *
 * @author jaime
 */
public class PlayWithGUI {
    /*
        Se crea un modelo, se obtiene la instancia única de la vista, se obtiene la 
        instancia única del controlador, se conectan estos elementos y se le pide 
        al controlador que arranque la aplicación.
    */
    public static void main(String[] args) {
        GameUniverse game = new GameUniverse();
        //View view = MainWindow.getInstance();
        DeepSpaceView view = MainWindow.getInstance();
        Controller controller = Controller.getInstance();
        controller.setModelView (game,view);
        controller.start(); 
    }
    
}
