/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Interface.java to edit this template
 */
package deepspace;

/**
 *
 * @author jaime
 */
public interface SpaceFighter {
    
    //Métodos que deben implementar las clases que realicen la interfaz
    
    //Realiza un disparo
    //Devuelve la potencia de disparo
    float fire();
    
    //Usa el escudo
    //Devuelve la potencia de escudo
    float protection();
    
    //Lo necesario cuando se recibe un disparo
    //Recibe la potencia de disparo
    //Devuelve el resultado del disparo
    ShotResult receiveShot(float shot);
    
    
}
