/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;


//Representa las armas de las que puede disponer una estacion espacial para 
//potenciar su energia al disparar.
class Weapon implements CombatElement{
    
    private String name;
    private WeaponType type;
    private int uses;
    
    Weapon (String name, WeaponType type, int uses){
        
        this.name=name;
        this.type=type;
        this.uses=uses;
    }
    
    Weapon (Weapon s){
        
        this.name=s.name;
        this.type=s.type;
        this.uses=s.uses;
    }
    
    
    WeaponToUI getUIversion(){
        
        return (new WeaponToUI(this));
        
    }
    
    
    public WeaponType getType(){
        
        return type;
    }
    
    //Implementamos el método de la interfaz
    @Override
    public int getUses(){
        
        return uses;
    }
    
    public float power(){
        
        return type.getPower();
    }
    
    //Implementamos el método de la interfaz
    @Override
    public float useIt(){
        
        float output;
        if (uses > 0){
            uses--;
            output=power();
        }
        else output=1.0f;
        
        return output;
    }
    
    public String toString(){
        
        String output= "Name: "        + name            + "\n" +
                       "Weapon Type: " + type.toString() + "\n" +
                       "Uses: "        + uses            + "\n";
        
        return output;
    }
    
    
}
