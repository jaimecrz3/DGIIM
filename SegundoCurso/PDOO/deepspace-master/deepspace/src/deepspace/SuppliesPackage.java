/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package deepspace;

//Esta clase representa a un paquete de suministros para una estación espacial. 
//Puede contener armamento, combustible y/o energía para los escudos.
class SuppliesPackage {
    
    private float ammoPower;
    private float fuelUnits;
    private float shieldPower;
    
    SuppliesPackage(float ammoPower, float fuelUnits, float shieldPower){
        
        this.ammoPower = ammoPower;
        this.fuelUnits = fuelUnits;
        this.shieldPower = shieldPower;
        
    }
    
    SuppliesPackage(SuppliesPackage s){
        
        this.ammoPower = s.ammoPower;
        this.fuelUnits = s.fuelUnits;
        this.shieldPower = s.shieldPower;
        
    }
    
    public float getAmmoPower(){
        
        return ammoPower;
        
    }
    
    public float getFuelUnits(){
        
        return fuelUnits;
        
    }
    
    public float getShieldPower(){
        
        return shieldPower;
        
    }
    
    public String toString (){
        
        String output = "Supplies Package attributes:\n"    +
                        "ammoPower="   + ammoPower   + "\n" +
                        "fuelUnits="   + fuelUnits   + "\n" +
                        "shieldPower=" + shieldPower + "\n";
        return output;
        
    }
    
}
