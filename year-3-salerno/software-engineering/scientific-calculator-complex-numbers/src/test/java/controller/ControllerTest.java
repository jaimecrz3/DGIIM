/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/UnitTests/JUnit5TestClass.java to edit this template
 */
package controller;

import model.Calcolatore;
import model.NumeroComplesso;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

/**
 * Test For controller class
 */
public class ControllerTest {    
    //Test complex number "complete"
    NumeroComplesso opp1;
    //Test complex number just with real part
    NumeroComplesso oppReal;
    //Test complex number with just complex part
    NumeroComplesso oppComplex;
    //Testz zero complex number
    NumeroComplesso oppZero;
    
    Controller instance;
    
    @BeforeEach
    public void setUp() {
        instance = new Controller();
        
        opp1 = new NumeroComplesso(-1,2);
        oppReal = new NumeroComplesso(10);
        oppComplex = new NumeroComplesso(0,10);
        oppZero = new NumeroComplesso(0);
        
        instance.stack.add(opp1);
        instance.mw.addToStack(instance.stack.lastElement().toString());
        instance.stack.add(oppReal);
        instance.mw.addToStack(instance.stack.lastElement().toString());
        instance.stack.add(oppComplex);
        instance.mw.addToStack(instance.stack.lastElement().toString());
        instance.stack.add(oppZero);
        instance.mw.addToStack(instance.stack.lastElement().toString());
    }

    /**
     * Test of substractVar method, of class Controller.
     */
    @Test
    public void testSubstractVar() {
        System.out.println("substractVar");
        
        //Test if there is no value in var
        instance.subtractVar(0);
        assertEquals(oppZero, instance.stack.lastElement());
        assertEquals(4,instance.stack.size());
        assertEquals(null,instance.variables.get(0));
        
        //Different substractions
        instance.variables.set(0, oppZero);
        instance.subtractVar(0);
        assertEquals(oppZero, instance.variables.get(0));
        assertEquals(3,instance.stack.size());
        instance.subtractVar(0);
        assertEquals(new NumeroComplesso(0,-10), instance.variables.get(0));
        assertEquals(2,instance.stack.size());
        instance.subtractVar(0);
        assertEquals(new NumeroComplesso(-10,-10), instance.variables.get(0));
        assertEquals(1,instance.stack.size());
        instance.subtractVar(0);
        assertEquals(new NumeroComplesso(-9,-12), instance.variables.get(0));
        assertEquals(0,instance.stack.size());
        
        //Test if there is no value in stack
        instance.subtractVar(0);
        assertEquals(new NumeroComplesso(-9,-12), instance.variables.get(0));
        assertEquals(0,instance.stack.size());        
    }

    /**
     * Test of addVar method, of class Controller.
     */
    @Test
    public void testAddVar() {
        System.out.println("addVar");
        
        //Test if there is no value in var
        instance.addVar(0);
        assertEquals(oppZero, instance.stack.lastElement());
        assertEquals(4,instance.stack.size());
        assertEquals(null,instance.variables.get(0));
        
        //Different addition
        instance.variables.set(0, oppZero);
        instance.addVar(0);
        assertEquals(oppZero, instance.variables.get(0));
        assertEquals(3,instance.stack.size());
        instance.addVar(0);
        assertEquals(new NumeroComplesso(0,10), instance.variables.get(0));
        assertEquals(2,instance.stack.size());
        instance.addVar(0);
        assertEquals(new NumeroComplesso(10,10), instance.variables.get(0));
        assertEquals(1,instance.stack.size());
        instance.addVar(0);
        assertEquals(new NumeroComplesso(9,12), instance.variables.get(0));
        assertEquals(0,instance.stack.size());
        
        //Test if there is no value in stack
        instance.addVar(0);
        assertEquals(new NumeroComplesso(9,12), instance.variables.get(0));
        assertEquals(0,instance.stack.size());
    }

    /**
     * Test of varToStack method, of class Controller.
     */
    @Test
    public void testVarToStack() {
        System.out.println("varToStack");
        
        int stackSize = instance.stack.size();
        
        //Test if there is no value in var
        instance.varToStack(0);
        assertEquals(oppZero,instance.stack.lastElement());
        assertEquals(stackSize,instance.stack.size());
        
        //Test of method
        instance.variables.set(0, oppReal);
        instance.varToStack(0);
        assertEquals(oppReal,instance.stack.lastElement());
        assertEquals(stackSize+1,instance.stack.size());
        assertEquals(null,instance.variables.get(0));
    }

    /**
     * Test of stackToVar method, of class Controller.
     */
    @Test
    public void testStackToVar() {
        System.out.println("stackToVar");
        
        int stackSize = instance.stack.size();
        NumeroComplesso n = instance.stack.lastElement();
        
        //Test of method
        instance.stackToVar(0);
        assertEquals(n,instance.variables.get(0));
        assertEquals(stackSize-1,instance.stack.size());
        
        //Test if there is no value at stack
        instance.stack.clear();
        instance.mw.cleanStack();
        instance.stackToVar(0);
        
        assertEquals(n,instance.variables.get(0));
        assertEquals(0,instance.stack.size());
    }

    /**
     * Test of addStack method, of class Controller.
     */
    @Test
    public void testAddStack() {
        System.out.println("addStack");
        
        int size = instance.stack.size();
        
        //Test of the method
        instance.addStack();
        assertEquals(new NumeroComplesso(0,10), instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        instance.addStack();
        assertEquals(new NumeroComplesso(10,10), instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        instance.addStack();
        assertEquals(new NumeroComplesso(9,12), instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        
        //Test if there arent enough numbers in the stack
        instance.addStack();
        assertEquals(new NumeroComplesso(9,12), instance.stack.lastElement());
        assertEquals(size,instance.stack.size());
    }

    /**
     * Test of substractStack method, of class Controller.
     */
    @Test
    public void testSubstractStack() {
        System.out.println("substractStack");
        
        int size = instance.stack.size();
        
        //Test of the method
        instance.subtractStack();
        assertEquals(new NumeroComplesso(0,10), instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        instance.subtractStack();
        assertEquals(new NumeroComplesso(10,-10), instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        instance.subtractStack();
        assertEquals(new NumeroComplesso(-11,12), instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        
        //Test if there arent enough numbers in stack
        instance.subtractStack();
        assertEquals(new NumeroComplesso(-11,12), instance.stack.lastElement());
        assertEquals(size,instance.stack.size());
    }

    /**
     * Test of multiplyStack method, of class Controller.
     */
    @Test
    public void testMultiplyStack() {
        System.out.println("multiplyStack");
        
        //add new sample numbers
        instance.stack.add(new NumeroComplesso(1,1)) ;
        instance.mw.addToStack(instance.stack.lastElement().toString());
        instance.stack.add(new NumeroComplesso(2,1));
        instance.mw.addToStack(instance.stack.lastElement().toString());
        instance.stack.add(new NumeroComplesso(-1,4));
        instance.mw.addToStack(instance.stack.lastElement().toString());
        instance.stack.add(new NumeroComplesso(8,5));
        instance.mw.addToStack(instance.stack.lastElement().toString());
        instance.stack.add(new NumeroComplesso(0.4,3.2));
        instance.mw.addToStack(instance.stack.lastElement().toString());
        
        int size = instance.stack.size();

        //Test some multiplications
        instance.multiplyStack();
        NumeroComplesso aux = Calcolatore.multiply(
                new NumeroComplesso(0.4,3.2), 
                new NumeroComplesso(8,5));
        assertEquals(aux, instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        instance.multiplyStack();
        aux = Calcolatore.multiply(
                aux, 
                new NumeroComplesso(-1,4));
        assertEquals(aux, instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        instance.multiplyStack();
        aux = Calcolatore.multiply(
                aux, 
                new NumeroComplesso(2,1));
        assertEquals(aux, instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        instance.multiplyStack();
        aux = Calcolatore.multiply(
                aux, 
                new NumeroComplesso(1,1));
        assertEquals(aux, instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        instance.multiplyStack();
        aux = Calcolatore.multiply(
                aux, 
                new NumeroComplesso(0,0));
        assertEquals(aux, instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        
        //Test multiplication with no numbers in stack
        instance.stack.clear();
        instance.mw.cleanStack();
        instance.stack.add(aux);
        instance.mw.addToStack(instance.stack.lastElement().toString());
        instance.multiplyStack();
        assertEquals(aux,instance.stack.lastElement());
        assertEquals(1,instance.stack.size());
    }

    /**
     * Test of divideStack method, of class Controller.
     */
    @Test
    public void testDivideStack() {
        System.out.println("divideStack");

        //add new sample numbers
        instance.stack.add(new NumeroComplesso(1,1)) ;
        instance.mw.addToStack(instance.stack.lastElement().toString());
        instance.stack.add(new NumeroComplesso(2,1));
        instance.mw.addToStack(instance.stack.lastElement().toString());
        instance.stack.add(new NumeroComplesso(-1,4));
        instance.mw.addToStack(instance.stack.lastElement().toString());
        instance.stack.add(new NumeroComplesso(8,5));
        instance.mw.addToStack(instance.stack.lastElement().toString());
        instance.stack.add(new NumeroComplesso(0.4,3.2));
        instance.mw.addToStack(instance.stack.lastElement().toString());
        
        int size = instance.stack.size();
        //Test some divisions
        instance.divideStack();
        NumeroComplesso aux = Calcolatore.divide(
                new NumeroComplesso(8,5), 
                new NumeroComplesso(0.4,3.2));
        assertEquals(aux, instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        instance.divideStack();
        aux = Calcolatore.divide(
                new NumeroComplesso(-1,4),
                aux);
        assertEquals(aux, instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        instance.divideStack();
        aux = Calcolatore.divide(
                new NumeroComplesso(2,1),
                aux);
        assertEquals(aux, instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        instance.divideStack();
        aux = Calcolatore.divide(
                new NumeroComplesso(1,1),
                aux);
        assertEquals(aux, instance.stack.lastElement());
        assertEquals(--size,instance.stack.size());
        
        //Test division by 0
        instance.divideStack();
        assertEquals("0", instance.stack.lastElement().toString());
        assertEquals(--size,instance.stack.size());
        
        //Test with no numbers in stack
        instance.stack.clear();
        instance.mw.cleanStack();
        instance.stack.add(aux);
        instance.mw.addToStack(instance.stack.lastElement().toString());
        instance.divideStack();
        assertEquals(aux,instance.stack.lastElement());
        assertEquals(1,instance.stack.size());
    }

    /**
     * Test of sqrtStack method, of class Controller.
     */
    @Test
    public void testSqrtStack() {
        System.out.println("sqrtStack");
       
        instance.stack.add(new NumeroComplesso(21,-31)) ;
        instance.mw.addToStack(instance.stack.lastElement().toString());
        
        int size = instance.stack.size();
        
        //Test some square roots
        NumeroComplesso n = Calcolatore.sqrt(instance.stack.lastElement());
        instance.sqrtStack();
        assertEquals(n,instance.stack.lastElement());
        assertEquals(size,instance.stack.size());
        n = Calcolatore.sqrt(instance.stack.lastElement());
        instance.sqrtStack();
        assertEquals(n,instance.stack.lastElement());
        assertEquals(size,instance.stack.size());
        n = Calcolatore.sqrt(instance.stack.lastElement());
        instance.sqrtStack();
        assertEquals(n,instance.stack.lastElement());
        assertEquals(size,instance.stack.size());

        //Test of square root of zero
        instance.stack.add(new NumeroComplesso(0,0));
        instance.mw.addToStack(instance.stack.lastElement().toString());
        size++;
        n = Calcolatore.sqrt(instance.stack.lastElement());
        instance.sqrtStack();
        assertEquals(n,instance.stack.lastElement());
        assertEquals(size,instance.stack.size());
        
        //Test of square root with no numbers in stack
        instance.stack.clear();
        instance.mw.cleanStack();

        instance.sqrtStack();
        assertEquals(0,instance.stack.size());

    }

    /**
     * Test of invertSign method, of class Controller.
     */
    @Test
    public void testInvertSign() {
        System.out.println("invertSign");
        
        instance.stack.add(new NumeroComplesso(21,-31)) ;
        instance.mw.addToStack(instance.stack.lastElement().toString());
        
        int size = instance.stack.size();
        
        //Test invert sign
        NumeroComplesso n = Calcolatore.invertSign(instance.stack.lastElement());
        instance.invertSign();
        assertEquals(n,instance.stack.lastElement());
        assertEquals(size,instance.stack.size());
        
        instance.stack.add(new NumeroComplesso(0,0)) ;
        instance.mw.addToStack(instance.stack.lastElement().toString());
        
        size = instance.stack.size();
        
        //Test with zero
        n = Calcolatore.invertSign(instance.stack.lastElement());
        instance.invertSign();
        assertEquals(n,instance.stack.lastElement());
        assertEquals(size,instance.stack.size());
        
        instance.stack.clear();
        instance.mw.cleanStack();

        //Test with no number at stack
        instance.invertSign();
        assertEquals(0,instance.stack.size());
    }

    /**
     * Test of inputHandler method, of class Controller.
     */
    @Test
    public void testInputHandler() {
        System.out.println("inputHandler");
    
        //Test random input
        String aux = "SDLKFLSKDMFL";
        
        instance.inputHandler(aux);
        assertEquals(4,instance.stack.size());

        //Test correct number input
        aux = "8";
        instance.inputHandler(aux);
        assertEquals(new NumeroComplesso(8),instance.stack.lastElement());
        assertEquals(5,instance.stack.size());
        
        aux = "8j";
        instance.inputHandler(aux);
        assertEquals(new NumeroComplesso(0,8),instance.stack.lastElement());
        assertEquals(6,instance.stack.size());
        
        aux = "1+8j";
        instance.inputHandler(aux);
        assertEquals(new NumeroComplesso(1,8),instance.stack.lastElement());
        assertEquals(7,instance.stack.size());
        
        
        //Test operation 
        aux = "+";
        instance.inputHandler(aux);
        assertEquals(new NumeroComplesso(1,16),instance.stack.lastElement());
        assertEquals(6,instance.stack.size());
        
        aux = "/";
        instance.inputHandler(aux);
        assertEquals(new NumeroComplesso(0.031,-0.498).toString(),instance.stack.lastElement().toString());
        assertEquals(5,instance.stack.size());
        
        aux = "-";
        instance.inputHandler(aux);
        assertEquals(new NumeroComplesso(-0.031,0.498).toString(),instance.stack.lastElement().toString());
        assertEquals(4,instance.stack.size());
        
        aux = "*";
        instance.inputHandler(aux);
        assertEquals(new NumeroComplesso(-4.981,-0.311).toString(),instance.stack.lastElement().toString());
        assertEquals(3,instance.stack.size());    
        
        aux = "+-";
        instance.inputHandler(aux);
        assertEquals(new NumeroComplesso(4.981,+0.311).toString(),instance.stack.lastElement().toString());
        assertEquals(3,instance.stack.size());
        
        aux = "sqrt";
        NumeroComplesso numAux = Calcolatore.sqrt(instance.stack.lastElement());
        instance.inputHandler(aux);
        assertEquals(numAux,instance.stack.lastElement());
        assertEquals(3,instance.stack.size());
        
        //Test variable operations
        aux = ">a";
        instance.inputHandler(aux);
        assertEquals(new NumeroComplesso(10),instance.stack.lastElement());
        assertEquals(2,instance.stack.size());
        assertEquals(numAux,instance.variables.get(0));
        
        aux = "+a";
        numAux = Calcolatore.add(numAux, instance.stack.lastElement());
        instance.inputHandler(aux);
        assertEquals(new NumeroComplesso(-1,2),instance.stack.lastElement());
        assertEquals(1,instance.stack.size());
        assertEquals(numAux,instance.variables.get(0)); 
        
        aux = "-a";
        numAux = Calcolatore.substract(instance.stack.lastElement(), numAux);
        instance.inputHandler(aux);
        assertEquals(0,instance.stack.size());
        assertEquals(numAux,instance.variables.get(0));
        
        aux = "<a";
        instance.inputHandler(aux);
        assertEquals(numAux,instance.stack.lastElement());
        assertEquals(1,instance.stack.size());
        assertEquals(null,instance.variables.get(0));
        
        //Test drop and drop with no number in stack
        aux = "drop";
        instance.inputHandler(aux);
        assertEquals(0,instance.stack.size());
        aux = "drop";
        instance.inputHandler(aux);
        assertEquals(0,instance.stack.size());
        
        numAux = new NumeroComplesso(7);
        
        instance.stack.add(numAux);
        instance.mw.addToStack(instance.stack.lastElement().toString());
        
        //Test clear and clear with no number in stack
        aux = "clear";
        instance.inputHandler(aux);
        assertEquals(0,instance.stack.size());
        aux = "clear";
        instance.inputHandler(aux);
        assertEquals(0,instance.stack.size());
        
        //Test dup with no number in stack and dup
        aux = "dup";
        instance.inputHandler(aux);
        assertEquals(0,instance.stack.size());
        instance.stack.add(numAux);
        instance.mw.addToStack(instance.stack.lastElement().toString());
        aux = "dup";
        instance.inputHandler(aux);
        assertEquals(2,instance.stack.size());
        assertEquals(numAux,instance.stack.lastElement());

        //Test swap
        numAux = new NumeroComplesso(9);
        instance.stack.add(numAux);
        instance.mw.addToStack(instance.stack.lastElement().toString());
        aux = "swap";
        instance.inputHandler(aux);
        assertEquals(3,instance.stack.size());
        assertEquals(new NumeroComplesso(7),instance.stack.lastElement());
        
        //Test over
        aux = "over";
        instance.inputHandler(aux);
        assertEquals(4,instance.stack.size());
        assertEquals(numAux,instance.stack.lastElement());
        
        //Incorrect number input

        aux = "1j+8j";
        instance.inputHandler(aux);
        assertEquals("9",instance.stack.lastElement().toString());
        assertEquals(4,instance.stack.size());
        
        aux = "1-6i";
        instance.inputHandler(aux);
        assertEquals("9",instance.stack.lastElement().toString());
        assertEquals(4,instance.stack.size());
        
        aux = "";
        instance.inputHandler(aux);
        assertEquals("9",instance.stack.lastElement().toString());
        assertEquals(4,instance.stack.size());
        
        aux = ".1+8j";
        instance.inputHandler(aux);
        assertEquals("9",instance.stack.lastElement().toString());
        assertEquals(4,instance.stack.size());
    }
    
}
