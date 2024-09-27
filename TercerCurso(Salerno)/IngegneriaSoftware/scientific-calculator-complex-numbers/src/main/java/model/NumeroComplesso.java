/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package model;

import java.text.DecimalFormat;

/**
 * This class represents a complex number. 
 * Handles creation, getters and toString.
 * 
 */
public class NumeroComplesso {
    
    private double realPart;
    private double imaginaryPart;
    
    /**
     * Constructor that takes a real number (double).
     * "Converts" a double to its value as a complex number.
     * 
     * @param real Real part of the number.
     */
    public NumeroComplesso(double real){
        realPart = real;
        imaginaryPart = 0;
    }
            
    /**
     * Constructor that takes real and complex part of the number.
     * 
     * @param real Real part of the number.
     * @param imaginary Complex part of the number.
     */
    public NumeroComplesso(double real,double imaginary){
        realPart = real;
        imaginaryPart = imaginary;
    }
    
    /**
     * Copy constructor.
     * @param n Complex number to copy.
     */
    public NumeroComplesso(NumeroComplesso n){
        realPart = n.realPart;
        imaginaryPart = n.imaginaryPart;
    }
    
    /**
     * Getter of real part.
     * @return 
     */
    public double getRealPart(){
        return(realPart);
    }
    
    /**
     * Getter of complex part.
     * @return 
     */
    public double getComplexPart(){
        return(imaginaryPart);
    }
    
    /**
     * Override of toString method.
     * @return Complex number in the format (a+bj)
     */
    @Override
    public String toString(){
        
        String sign = "+";
        String realpart = "";
        String imaginarypart = "";
        DecimalFormat numberFormat = new DecimalFormat("#.###");

        //Checks if complex part is negative
        if((int)(imaginaryPart*1000) < 0){
            sign = "-";
        }else if((int)(imaginaryPart*1000) == 0)
            sign = "";
        
        //Checks if there is real part
        if((int)(realPart*1000) != 0){
            realpart = numberFormat.format(realPart);
        }
        else if((int)(this.imaginaryPart*1000) == 0){
            realpart = "0";
        }
        
        if((int)(imaginaryPart*1000) != 0){
            imaginarypart = numberFormat.format(Math.abs(imaginaryPart)) + "j";
        }
        
        return(realpart + sign + imaginarypart);
        
    }
    
    /**
     * Calculates the module of the number.
     * 
     * @return Returns the module.
     */
    public double getModule(){    
        return(Math.sqrt((realPart*realPart)+(imaginaryPart*imaginaryPart)));    
    }
    
    /**
     * Calculate the angle of the number.
     * 
     * @return 
     */
    public double getAngle(){
        
        double angle;
        
        if(realPart==0&&imaginaryPart==0){
            angle=0;
        }else if(realPart==0){
            if(imaginaryPart>0){
                angle = Math.PI/2;
            }else{
                angle = (3*Math.PI)/2;
            }
        }else if(realPart>0&&imaginaryPart>=0){
            angle=Math.atan(imaginaryPart/realPart);
        }else if(realPart<0&&imaginaryPart>=0){
            angle=Math.PI - Math.atan(imaginaryPart/Math.abs(realPart));
        }else if(realPart<0&&imaginaryPart<=0){
            angle=Math.PI + Math.atan(Math.abs(imaginaryPart)/Math.abs(realPart));
        }else{
            angle=2*Math.PI - Math.atan(Math.abs(imaginaryPart)/realPart);
        }
        
        return(angle);   
    }
    
    
    /**
     * Override of equal method.
     * Useful to compare complex numbers.
     * 
     * @param o Object to be compared
     * @return True if numbers are equal.
     */
    @Override
    public boolean equals(Object o) {
 
        // If the object is compared with itself then return true  
        if (o == this) {
            return true;
        }
 
        /* Check if o is an instance of Complex or not
          "null instanceof [type]" also returns false */
        if (!(o instanceof NumeroComplesso)) {
            return false;
        }
         
        // typecast o to Complex so that we can compare data members 
        NumeroComplesso c = (NumeroComplesso) o;
         
        // Compare the data members and return accordingly 
        return (this.getRealPart()==c.getRealPart())
                && (this.getComplexPart()==c.getComplexPart());
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 97 * hash + (int) (Double.doubleToLongBits(this.realPart) ^ (Double.doubleToLongBits(this.realPart) >>> 32));
        hash = 97 * hash + (int) (Double.doubleToLongBits(this.imaginaryPart) ^ (Double.doubleToLongBits(this.imaginaryPart) >>> 32));
        return hash;
    }
    
}
