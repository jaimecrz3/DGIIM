package model;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

/**
 *
 * @author fpcolacino
 */

 public class CalcStackTest {
 
     private CalcStack<Integer> calcStack;
    
    @BeforeEach
    public void setUp() {
        calcStack = new CalcStack<>();
    }

 /*
 * Test of add method, of class CalcStack.
 */


    // Testa il metodo drop per verificare che questo rimuova 
    // correttamente l'elemento in cima dallo stack.
    @Test
    public void testDrop() {
        calcStack.push(1);
        calcStack.push(2);
        assertEquals(2, calcStack.drop());
        assertEquals(1, calcStack.size());
    }


    // Testa dup per verificare che duplichi correttamente 
    // l'elemento che si trova in cima allo stack.
    @Test
    public void testDup() {
        calcStack.push(3);
        assertTrue(calcStack.dup());
        assertEquals(2, calcStack.size());
        assertEquals(3, calcStack.drop());
        assertEquals(3, calcStack.drop());
    }


    // Testa il metodo dup su uno stack vuoto in modo che 
    // restituisca false e mantenga lo stack vuoto.
    @Test
    public void testDupEmptyStack() {
        assertFalse(calcStack.dup());
        assertEquals(0, calcStack.size());
    }

    // Testa il metodo swap per verificare lo scambio corretto 
    // degli elementi in cima allo stack.
    @Test
    public void testSwap() {
        calcStack.push(4);
        calcStack.push(5);
        assertTrue(calcStack.swap());
        assertEquals(2, calcStack.size());
        assertEquals(4, calcStack.drop());
        assertEquals(5, calcStack.drop());
    }


    // Testa il metodo swap() su uno stack con meno di due elementi, 
    // dovrebbe restituire false e mantenere lo stack inalterato.
    @Test
    public void testSwapNotEnoughElements() {
        calcStack.push(6);
        assertFalse(calcStack.swap());
        assertEquals(1, calcStack.size());
    }


    // Testa il metodo over per verificare la duplicazione corretta 
    // del secondo elemento dal top dello stack.
    @Test
    public void testOver() {
        calcStack.push(7);
        calcStack.push(8);
        assertTrue(calcStack.over());
        assertEquals(3, calcStack.size());
        assertEquals(7, calcStack.pop());
        assertEquals(8, calcStack.pop());
        assertEquals(7, calcStack.pop());
    }


    // Testa il metodo over() su uno stack con meno di due elementi, 
    // dovrebbe restituire false e mantenere lo stack inalterato.
    @Test
    public void testOverNotEnoughElements() {
        calcStack.push(9);
        assertFalse(calcStack.over());
        assertEquals(1, calcStack.size());
    }
}
