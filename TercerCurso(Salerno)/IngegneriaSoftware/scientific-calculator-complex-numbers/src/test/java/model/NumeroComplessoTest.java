package model;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

/*
 *
 * @author fpcolacino
 */

  
 public class NumeroComplessoTest {
 
     private NumeroComplesso numeroComplesso;
     
    public NumeroComplessoTest() {
    }
    
    @BeforeAll
    public static void setUpClass() {
    }
    
    @AfterAll
    public static void tearDownClass() {
    }
    
    @BeforeEach
    public void setUp() {
        numeroComplesso = new NumeroComplesso(3,4);
    }
    
    @AfterEach
    public void tearDown() {
    }
 

    /*
     * Test of add method, of class Calcolatore.
     */

    // Testa il metodo getRealPart per verificare che restituisca correttamente la parte reale del numero complesso.
     @Test
    public void testGetRealPart() {
         assertEquals(3.0, numeroComplesso.getRealPart());
     }
 

    // Testa il metodo getComplexPart per verificare che restituisca correttamente la parte immaginaria del numero complesso.
     @Test
    public void testGetComplexPart() {
         assertEquals(4.0, numeroComplesso.getComplexPart());
     }
 
    // Testa il metodo toString per verificare che restituisca correttamente la rappresentazione in stringa del numero complesso.
     @Test
    public void testToString() {
         assertEquals("3+4j", numeroComplesso.toString());
     }
 
    
     @Test
    public void testGetModule() {
         assertEquals(5.0, numeroComplesso.getModule());
     }

    // Testa il metodo getAngle per verificare che restituisca correttamente l'angolo del numero complesso.
     @Test
    public void testGetAngle() {
         assertEquals(0.93, numeroComplesso.getAngle(), 0.01); // L'angolo dovrebbe essere approssimativamente 0.93 rad.
     }
 }
 
