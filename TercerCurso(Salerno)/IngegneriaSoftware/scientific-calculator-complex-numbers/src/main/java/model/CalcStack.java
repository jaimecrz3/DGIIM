/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package model;

import java.util.Stack;

/**
 * This class implements a few methods to the stack 
 * that modifies the last elements of itself.
 * 
 * @param <T> 
 */
public class CalcStack<T> extends Stack<T>{
    
    /**
     * This method eliminates the last element of the stack.
     * @return The element eliminated.
     */
    public T drop(){
        return this.pop();
    }
    
    /**
     * This method duplicates the last element of the stack.
     * @return True if the stack isn't empty.
     */
    public boolean dup(){
        boolean canDup = false;
        if(!this.isEmpty()){
            canDup = true;
            this.add(this.lastElement());
        }
        
        return canDup;
    }
    
    /**
     * This method swaps the last two elements of the stack.
     * @return True if there is at least two elements.
     */
    public boolean swap(){
        boolean canSwap = false;
        
        if(this.size() >= 2){
            canSwap = true;
            
            T aux = this.drop();
            T aux2 = this.drop();
            this.add(aux);
            this.add(aux2);
        }
        
        return canSwap;
    }
    
    /**
     * This method duplicates the second last element in the stack.
     * @return True if there is at least two elements in the stack.
     */
    public boolean over(){
        boolean canOver = false;
        
        if(this.size() >= 2){
            canOver = true;
            
            T aux = this.drop();
            T aux2 = this.lastElement();
            
            this.add(aux);
            this.add(aux2);
        }
        
        return canOver;
    }
}
