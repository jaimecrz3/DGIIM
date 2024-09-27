/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Interface.java to edit this template
 */
package deepspace;

/**
 *
 * @author jaime
 */
public interface CombatElement {
    
    //Métodos que deben implementar las clases que realicen la interfaz
    
    //Devuelve los usos restantes
    int getUses();
    
    //Para usar el elemento
    float useIt();
    
    
}
