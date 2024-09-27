/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;

//Esta clase representa a una nave espacial enemiga.
public class EnemyStarShip implements SpaceFighter{
    
    
    //Nombre.
    private String name;
    
    //Poder de disparo.
    private float ammoPower;
    
    //Poder de escudo.
    private float shieldPower;
    
    //Botin.
    private Loot loot;
    
    //Daño que puede hacer.
    private Damage damage;
    
    //Constructores
    EnemyStarShip(String n, float a, float s, Loot l, Damage d){
        
        name=n;
        ammoPower=a;
        shieldPower=s;
        loot=l;
        damage=d;
    }
    
    EnemyStarShip(EnemyStarShip e){this(e.name,e.ammoPower,e.shieldPower,e.loot,e.damage);}
    
    
    //Interfaz
    EnemyToUI getUIversion(){return (new EnemyToUI(this));}
    
    
    //Getters
    
    //Implementamos el método de la interfaz
    @Override
    public float fire(){
        return ammoPower;
    }
    
    public float getAmmoPower(){return ammoPower;}
    
    public Damage getDamage(){return damage;}
    
    public Loot getLoot(){return loot;}
    
    public String getName(){return name;}
    
    public float getShieldPower(){return shieldPower;}
    
    //Implementamos el método de la interfaz
    @Override
    public float protection(){
        return shieldPower;
    }
    
    
    //Others
    
    /*
    Devuelve el resultado que se produce al recibir un disparo de una determinada 
    potencia (pasada como parámetro). 
    
    Si el nivel de la protección de los escudos es menor que la intensidad del 
    disparo, la nave enemiga no resiste (DONOTRESIST).
    En casocontrario resiste el disparo (RESIST).
    */
    //Implementamos el método de la interfaz
    @Override
    public ShotResult receiveShot(float shot){
        
        ShotResult result=ShotResult.RESIST;
        
        if (shieldPower < shot) result=ShotResult.DONOTRESIST;
        
        return result;
    }

    public String toString(){
        
        String output= "name: "             + name           + "\n" +
                       "ammoPower: "        + ammoPower          + "\n" +
                       "shieldPower: "      + shieldPower + "\n" +
                        "Loot: "            + loot.toString() +
                        "Damage: "          + damage.toString();
        
        return output;
    }
    
    
}
