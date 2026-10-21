/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package model;


/**
 * This class holds basic operations of complex numbers.
 */
public class Calcolatore {
    
    /**
     * Adds two complex numbers.
     * 
     * @param n1 One complex number.
     * @param n2 One complex number.
     * @return The result of the operation as a new number.
     */
    public static NumeroComplesso add(NumeroComplesso n1, NumeroComplesso n2){
        double parte_real = n2.getRealPart() + n1.getRealPart();
        double parte_imaginary = n2.getComplexPart() + n1.getComplexPart();
        
        NumeroComplesso result = new NumeroComplesso(parte_real, parte_imaginary);
        
        return(result);
    }
    
    /**
     * Subtracts two complex numbers.
     * 
     * @param n1 Minuend.
     * @param n2 Subtrahend.
     * @return The result of the operation as a new number.
     */
    public static NumeroComplesso substract(NumeroComplesso n1, NumeroComplesso n2){
        double parte_real = n2.getRealPart() - n1.getRealPart();
        double parte_imaginary = n2.getComplexPart() - n1.getComplexPart();
        
        NumeroComplesso result = new NumeroComplesso(parte_real, parte_imaginary);
        
        return(result);
    }
    
    /**
     * Multiplicate two complex numbers.
     * 
     * @param n1 One complex number.
     * @param n2 One complex number.
     * @return The result of the operation as a new number.
     */
    public static NumeroComplesso multiply(NumeroComplesso n1, NumeroComplesso n2){
        double parte_real = n2.getRealPart() * n1.getRealPart() - 
                (n1.getComplexPart() * n2.getComplexPart());
        double parte_imaginary = n1.getRealPart()*n2.getComplexPart() + 
                n1.getComplexPart()* n2.getRealPart();
        
        NumeroComplesso result = new NumeroComplesso(parte_real, parte_imaginary);
        
        return(result);
    }
    
    /**
     * Divide two complex numbers.
     * 
     * @param n1 Dividend.
     * @param n2 Divisor.
     * @return The result of the operation as a new number.
     */
    public static NumeroComplesso divide(NumeroComplesso n1, NumeroComplesso n2){
        
        double denominador = n2.getRealPart()*n2.getRealPart() + 
                             n2.getComplexPart()*n2.getComplexPart();
        
        double parte_real = (n2.getRealPart()*n1.getRealPart() + 
                             n2.getComplexPart()*n1.getComplexPart())/denominador;
        
        double parte_imaginary = (n1.getComplexPart()*n2.getRealPart() - 
                             n1.getRealPart()*n2.getComplexPart())/denominador;
        
        NumeroComplesso result = new NumeroComplesso(parte_real, parte_imaginary);
        return(result);
        
    }
    
    /**
     * Inverts the sign of a complex number.
     * 
     * @param n1 One complex number
     * @return The result of the operation as a new number.
     */
    public static NumeroComplesso invertSign(NumeroComplesso n1){
        
        int cambio = -1;
        double parte_real = n1.getRealPart()*cambio;
        double parte_imaginary = n1.getComplexPart()*cambio;
        NumeroComplesso result = new NumeroComplesso(parte_real, parte_imaginary);
        return(result);
        
    }
    
    /**
     * Do the square root of a complex number.
     * 
     * @param n1 One complex number.
     * @return The result of the operation as a new number.
     */
    public static NumeroComplesso sqrt(NumeroComplesso n1){
        
        double module = Math.sqrt(n1.getModule());
        double arg = n1.getAngle()/2;
        
        double parte_real = module*Math.cos(arg);
        double parte_imaginary = module*Math.sin(arg);
        NumeroComplesso result = new NumeroComplesso(parte_real, parte_imaginary);
        
        if(result.getRealPart() < 0)
            result = invertSign(result);
        
        return(result);
    }
    
}
