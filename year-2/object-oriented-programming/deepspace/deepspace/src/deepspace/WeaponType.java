/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;

//Este enumerado representa a los tipos de armas del juego. Cada valor posible 
//tendrá asociado un valor numérico concreto igual a la potencia de disparo de 
//cada tipo de arma.
public enum WeaponType {
    
    LASER(2.0f), 
    MISSILE(3.0f), 
    PLASMA(4.0f); 
    
    private final float power;
    
    WeaponType(float power){
        
        this.power = power;
        
    }
    
    float getPower(){
        
        return power;
        
    }
    
    public String toString(){
        
        String output;
        if (power==2.0f) output="LASER";
        else if (power==3.0f) output ="MISSILE";
        else output= "PLASMA";
                
        return output;
    }
    
}
