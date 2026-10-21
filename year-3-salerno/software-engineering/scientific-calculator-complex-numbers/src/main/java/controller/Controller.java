/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package controller;

import View.MainView;
import java.util.ArrayList;
import model.NumeroComplesso;
import model.CalcStack;
import model.Calcolatore;

/**
 *
 * @author gabriel
 * 
 * This class put every part of the program together: 
 *  Calcolatore, variables, stack and Interface
 * 
 * It is responsible to:
 *  - Handle input
 *  - Handle errors
 *  - Process every operation and delegate to the correct class
 */
public class Controller {
    
    public ArrayList<NumeroComplesso> variables;
    public CalcStack<NumeroComplesso> stack;
    public MainView mw;
    
    /**
     * Constructor of Controller
     * Initialize the interface, the stack and variables
     */
    public Controller(){
        mw = new MainView();
        mw.setController(this);
        stack = new CalcStack<>();
        variables = new ArrayList<>();
        for(int i = 'a'; i <= 'z'; ++i){
            variables.add(null);
        }
    }
    
    /**
     * This method checks if a string is a complex number (NumeroComplesso).
     * The format of a complex number is a+bj, a b integer numbers.
     * If it is, inserts the number into stack.
     * 
     * @param input The string to be checked
     * @return True if the input is a complex number, false if not.
     */
    private boolean isNumeric(String input){
        
        boolean is_num = false;
        
        //Checks if the input is empty and if the first character is correct
        if(input!=null && !input.isEmpty() && 
                ((input.charAt(0)>='0'&&input.charAt(0)<='9')
                ||((input.charAt(0)=='+'||
                input.charAt(0)=='-')&&input.length()>1))){
            
            int counter=0;
            is_num=true;
            //For avoid something like this: 4j+7j
            boolean only_complex_part=false;
            String real_part = "";
            String complex_part = "";
            
            
            //Checks if the first character is a sign operator
            if(input.charAt(0)=='+' || input.charAt(0)=='-'){
                real_part += input.charAt(0);
                counter++;
            }
            
            /*            
            if(input.charAt(counter)<'0'|| input.charAt(counter)>'9'){
            is_num = false;
            }*/
            
            boolean comma = false;
            //Checks if the first part of the number is numeric
            for(; ((input.charAt(counter)>='0'&&
                    input.charAt(counter)<='9')||
                    (input.charAt(counter)=='.'&&!comma))&& 
                    counter <input.length()-1;counter++){
                
                real_part += input.charAt(counter);
                //Checks if the input has a double as the real part
                if(input.charAt(counter)==','){
                    comma = true;
                }
            }
            if(input.charAt(counter)>='0'&&input.charAt(counter)<='9'){
                real_part += input.charAt(counter);
                counter++;
            }
            
            //checks if the number is a pure complex number (just complex part)
            if(counter < input.length()){
                if(input.charAt(counter)=='j'){
                    complex_part = real_part;
                    real_part = "0";
                    ++counter;
                    only_complex_part=true;
                }
            }
            
            //Checks the second part
            if(counter<input.length()){
                
                if(((input.charAt(counter)=='+'||
                        input.charAt(counter)=='-')
                    &&!real_part.isEmpty())){
                
                    //checks if the second part start with a sign operator
                    if(input.charAt(counter)=='+' || 
                            input.charAt(counter)=='-'){
                        complex_part += input.charAt(counter);
                        counter++;
                    }
                    
                    if(counter<input.length()){
                        if(input.charAt(counter)>='0'&&
                                input.charAt(counter)<='9'){

                            comma = false;
                            //Checks if the second part is numeric
                            for(; ((input.charAt(counter)>='0'&&
                                    input.charAt(counter)<='9')||
                                    (input.charAt(counter)=='.'&&!comma))&&
                                    counter<input.length()-1;counter++){
                                
                                complex_part += input.charAt(counter);
                                //Checks if the input has a double as the complex part
                                if(input.charAt(counter)==','){
                                    comma = true;
                                }
                                
                            }
                            if(input.charAt(counter)>='0'&&
                                    input.charAt(counter)<='9'){
                                complex_part += input.charAt(counter);
                                ++counter;
                            }

                            //checks if the second part has the "j" 
                            //to indicate it is a complex number
                            if(counter!=input.length()-1||
                                    input.charAt(counter)!='j'||only_complex_part){
                                is_num = false;
                            }

                        } else{

                            is_num = false;

                        }
                    } else{
                        is_num = false;
                    }
                    
                } else{

                    is_num = false;

                }
            }
            
            //If the input is a complex number, insert it in the stack
            if(is_num){
                if(complex_part.isEmpty()){
                    stack.add(new NumeroComplesso(Double.parseDouble(real_part)));
                }else{
                    stack.add(new NumeroComplesso(Double.parseDouble(real_part), 
                            Double.parseDouble(complex_part)));
                }
                
                mw.addToStack(stack.lastElement().toString());
                
            }
            
             
        }
        
        return(is_num);
        
    }
    
    /**
     * Checks if the input is a complex number operation.
     * The operations are:
     *  - Addition          +
     *  - Subtraction       -
     *  - Multiplication    *
     *  - Division          /
     *  - Square root       sqrt
     *  - Invert sign       +-
     * If the input is an operation, it process it.
     * 
     * @param input String to be checked.
     * @return True if the input is operation, false if not.
     */
    private boolean isCalculatorOpValid(String input){
        
        boolean es_op = false;
        
        //Switch to check if its a operation
        if(input!=null && !input.isEmpty()){
            switch(input){
                case "+":
                    addStack();
                    es_op=true;
                    break;
                case "-":
                    subtractStack();
                    es_op=true;
                    break;
                case "*":
                    multiplyStack();
                    es_op=true;
                    break;
                case "/":
                    divideStack();
                    es_op=true;
                    break;
                case "sqrt":
                    sqrtStack();
                    es_op=true;
                    break;
                case "+-":
                    invertSign();
                    es_op=true;
                    break;    
                    
            }
            
        }
        
        return(es_op);
        
    }
    
    /**
     * This method checks if the input is a stack operator.
     * The valid operators for stack is:
     *  - clear
     *  - drop
     *  - dup
     *  - swap
     *  - over
     * If the input is an operation, it process it.
     * 
     * It is responsible to send a error message to 
     * the interface if the operations are invalid
     * 
     * @param input String to be checked.
     * @return True if the input is operation, false if not.
     */
    private boolean isStackOpValid(String input){
        
        boolean es_op = false;
        
        //Switch to check if its a operation
        if(input!=null && !input.isEmpty()){
            switch(input){
                case "clear":
                    stack.clear();
                    es_op=true;
                    mw.cleanStack();
                    break;
                case "drop":
                    
                    es_op=true;
                    if(!stack.empty()){
                        stack.drop();
                        mw.drop();
                    }
                    else
                        mw.NoValueInStackToOperateMessage();
                    break;
                case "dup":
                    es_op=true;
                    if(stack.dup())
                        mw.dup(stack.lastElement().toString());
                    else
                        mw.NoValueInStackToOperateMessage();
                    break;
                case "swap":
                    NumeroComplesso n1 = stack.lastElement();
                    es_op=true;
                    if(stack.swap())
                        mw.swap(n1.toString(), stack.lastElement().toString());
                    else
                        mw.NoValueInStackToOperateMessage();
                    break;
                case "over":
                    es_op=true;
                    if(stack.over())
                        mw.over(stack.lastElement().toString());
                    else
                        mw.NoValueInStackToOperateMessage();
                    break;    
                    
            }
            
        }
        
        return(es_op);
        
    }
    
    /**
     * This method checks if the input is a variable operation.
     * the variable operations are:
     *  - Stack to var 
     *  - Var to stack 
     *  - Add to var 
     *  - Subtract to var
     * If the input is an operation, it process it.
     * 
     * @param input String to be checked.
     * @return True if the input is operation, false if not.
     */
    private boolean isVarOpValid(String input){
        
        boolean es_op = false;

        //Check if its a operation
        if(input!=null && !input.isEmpty() && input.length()==2){
            if(input.charAt(0)=='>' && input.charAt(1)>='a' && input.charAt(0)<='z'){
                
                stackToVar(input.charAt(1)-'a');
                es_op = true;
                
            }else if(input.charAt(0)=='<' && input.charAt(1)>='a' && input.charAt(0)<='z'){
                
                varToStack(input.charAt(1)-'a');
                es_op = true;
                
            }else if(input.charAt(0)=='+' && input.charAt(1)>='a' && input.charAt(0)<='z'){
                
                addVar(input.charAt(1)-'a');
                es_op = true;
                
            }else if(input.charAt(0)=='-' && input.charAt(1)>='a' && input.charAt(0)<='z'){
                
                subtractVar(input.charAt(1)-'a');
                es_op = true;
                
            }
            
            
        }
        
        return(es_op);
        
    }
    
    /**
     * Private method to get the value of a var given the index.
     * 
     * @param var Index of the variable. 
     * @return The value of the variable if the index is correct.
     */
    private NumeroComplesso getVar(int var){
        
        return(variables.get(var));
        
    }
    
    /**
     * Private method to set the value of a var given the index.
     * It is responsible to update the interface
     * @param var   The index of the variable to be changed.
     * @param n     The new value of the variable.
     */
    private void setVar(int var, NumeroComplesso n){
        variables.set(var, n);
        mw.setVar(var, n.toString());
        mw.removeFromStack();            
        mw.updateView();
    }
    
    
    
    public void subtractVar(int var){
        
        if(variables.get(var)!=null&&!stack.isEmpty()){
            NumeroComplesso n1 = variables.get(var);
            NumeroComplesso n2 = stack.drop();
            
            NumeroComplesso n = Calcolatore.substract(n2,n1);
            this.setVar(var, n);
            
        }else if(stack.isEmpty()){
            mw.NoValueInStackToOperateMessage();
        }else {
            mw.NoValueInVarToOperateMessage();
        }
        
        
    }
    
    public void addVar(int var){
        
        if(variables.get(var)!=null&&!stack.isEmpty()){
            NumeroComplesso n1 = stack.drop();
            NumeroComplesso n2 = variables.get(var);

            NumeroComplesso n = Calcolatore.add(n1, n2);
            this.setVar(var, n);
            
        }else if(stack.isEmpty()){
            mw.NoValueInStackToOperateMessage();
        }else {
            mw.NoValueInVarToOperateMessage();
        }
        
        
    }
    
    public void varToStack(int var){
        
        NumeroComplesso n = getVar(var);
        if(n!=null){
            mw.addToStack(n.toString());
            stack.add(n);
            this.variables.set(var, null);
            mw.setVar(var, Character.toString(var+'a'));           
            mw.updateView();
            
        }else{
            mw.NoValueInVarToOperateMessage();
        }
        
    }
    
    public void stackToVar(int var){
        
        if(!stack.isEmpty()){
            NumeroComplesso n = stack.drop();
            setVar(var, n);
        }else{
            mw.NoValueInStackToOperateMessage();
        }
        
    }
    
    public void addStack(){
        
        if(!stack.isEmpty()){
            
            NumeroComplesso n1 = stack.drop();
            
            if(!stack.isEmpty()){
                
                NumeroComplesso n2 = stack.drop();
                stack.add(Calcolatore.add(n1,n2));
                mw.processBinaryOp(stack.lastElement().toString());
                
            }else{
                
                stack.add(n1);
                mw.NoValueInStackToOperateMessage();
                
            }
            
            
        }else{
            mw.NoValueInStackToOperateMessage();
        }
        
    }
    
    public void subtractStack(){
        
        if(!stack.isEmpty()){
            
            NumeroComplesso n1 = stack.drop();
            
            if(!stack.isEmpty()){
                
                NumeroComplesso n2 = stack.drop();
                stack.add(Calcolatore.substract(n1,n2));
                mw.processBinaryOp(stack.lastElement().toString());
                
            }else{
                
                stack.add(n1);
                mw.NoValueInStackToOperateMessage();
                
            }
            
            
        }else{
            mw.NoValueInStackToOperateMessage();
        }
        
    }
    
    public void multiplyStack(){
        
        if(!stack.isEmpty()){
            
            NumeroComplesso n1 = stack.drop();
            
            if(!stack.isEmpty()){
                
                NumeroComplesso n2 = stack.drop();
                stack.add(Calcolatore.multiply(n1,n2));
                mw.processBinaryOp(stack.lastElement().toString());
                
            }else{
                
                stack.add(n1);
                mw.NoValueInStackToOperateMessage();
                
            }
            
            
        }else{
            mw.NoValueInStackToOperateMessage();
        }
        
    }
    
    public void divideStack(){
        
        if(!stack.isEmpty()){
            
            NumeroComplesso n1 = stack.drop();
            
            if(n1.getRealPart() == 0 && n1.getComplexPart() == 0){
                mw.CantDivideByZeroMessage();
                stack.add(n1);
            }else if(!stack.isEmpty()){
                NumeroComplesso n2 = stack.drop();
                
                NumeroComplesso n3 = Calcolatore.divide(n2,n1);
                stack.add(n3);
                mw.processBinaryOp(stack.lastElement().toString());
            }else{
                
                stack.add(n1);
                mw.NoValueInStackToOperateMessage();
                
            }
               
        }else{
            mw.NoValueInStackToOperateMessage();
        }
        
    }
    
    public void sqrtStack(){
        
        if(!stack.isEmpty()){
            
            NumeroComplesso n1 = stack.drop();
            stack.add(Calcolatore.sqrt(n1));
            mw.processUnaryOp(stack.lastElement().toString());
             
        }else{
            mw.NoValueInStackToOperateMessage();
        }
        
    }
    
    public void invertSign(){
        if(!stack.isEmpty()){
            
            NumeroComplesso n1 = stack.drop();
            stack.add(Calcolatore.invertSign(n1));
            mw.processUnaryOp(stack.lastElement().toString());
             
        }else{
            mw.NoValueInStackToOperateMessage();
        }
        
    }
    
    public void inputHandler(String s){
        
        boolean is_valid = isNumeric(s);

        if(!is_valid){
            
            is_valid = isCalculatorOpValid(s);
            if(!is_valid){
                
                is_valid = isStackOpValid(s);
                if(!is_valid){
                    
                    is_valid = isVarOpValid(s);
                    if(!is_valid){
                        
                        mw.InputErrorMessage();
                        
                    }
                    
                }
                
            }
            
            
        }
        
    }
    
    public void show(){
        mw.showView();
    }
    
    public void finish (int i) {
        if (mw.confirmExitMessage()) {
          System.exit(i);
        }
    }
    
    
}
