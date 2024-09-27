/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;


//Representa los potenciadores de escudo que pueden tener las estaciones 
//espaciales.
class ShieldBooster implements CombatElement{
    
    private String name;
    private float boost;
    private int uses;
    
    ShieldBooster(String name, float boost, int uses){
        
        this.name=name;
        this.boost=boost;
        this.uses=uses;
    }
    
    ShieldBooster(ShieldBooster s){
        
        this.name=s.name;
        this.boost=s.boost;
        this.uses=s.uses;
    }
    
    
    ShieldToUI getUIversion(){
        
        return (new ShieldToUI(this));
        
    }
    
    
    public float getBoost(){
        
        return boost;
    }
    
    //Implementamos el método de la interfaz
    @Override
    public int getUses(){
        
        return uses;
    }
    
    //Implementamos el método de la interfaz
    @Override
    public float useIt(){

        float output;
        if (uses > 0){
            uses--;
            output=boost;
        }

        else output=1.0f;

        return output;
    }
    

    public String toString (){
        
        String output = "\nShieldBooster attributes:\n" +
                        "\nboost=" + boost + "\n" +
                        "\nuses="  + uses  + "\n" +
                        "\nname="  + name  + "\n";
        return output;
        
    }
    
    
}
