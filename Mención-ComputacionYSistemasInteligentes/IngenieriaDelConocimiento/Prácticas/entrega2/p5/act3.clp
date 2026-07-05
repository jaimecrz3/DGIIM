;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; act3.clp
;;;; Autores: Jaime Corzo Galdo y Mario Lindez Martinez
;;;; Ingenieria del Conocimiento - Practica 5 - Actividad 3
;;;; Sistema difuso para estimar el riesgo de infarto
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FUNCIONES NECESARIAS DEL SISTEMA DIFUSO
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Calcula el grado de pertenencia de ?value al conjunto trapezoidal (?a ?b ?c ?d)
(deffunction membership (?value ?a ?b ?c ?d)
   (if  (< ?value ?a) then (bind ?rv 0)
      else
         (if (< ?value ?b) then (bind ?rv (/ (- ?value ?a) (- ?b ?a)))
            else
               (if  (< ?value ?c) then (bind ?rv 1)
                  else
                     (if (< ?value ?d) then (bind ?rv (/ (- ?d ?value) (- ?d ?c)))
                           else (bind ?rv 0)
                     )
               )
         )
   )
   ?rv
)

; Calcula el centro de gravedad aproximado del conjunto trapezoidal (?a ?b ?c ?d)
(deffunction center_of_gravity (?a ?b ?c ?d)
   (bind ?ati (/ (- ?b ?a) 2))
   (bind ?atd (/ (- ?d ?c) 2))
   (bind ?rv (/ (+ (+ ?b ?c) (- ?atd ?ati)) 2))
   ?rv)

; Conjuncion difusa. Se mantiene el producto, como en el sistema de ayuda.
(deffunction conjuncion (?x ?y)
   (bind ?rv (* ?x ?y))
   ?rv)

; Nueva función que transforma cualquier número a string limpio eludiendo el bug regional
(deffunction formatear-decimal (?x)
   (if (= ?x 0) then (return "0.00"))
   (bind ?signo "")
   (if (< ?x 0) then
      (bind ?signo "-")
      (bind ?x (abs ?x)))
   (bind ?total (integer (round (* ?x 100))))
   (bind ?entero (div ?total 100))
   (bind ?dec (mod ?total 100))
   (if (< ?dec 10)
      then (return (str-cat ?signo ?entero ".0" ?dec))
      else (return (str-cat ?signo ?entero "." ?dec)))
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; METODO DE DEFUZZIFICACION
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(deffacts Metodo_defuzzificacion
   (metodo defuzzificacion COS)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; CONOCIMIENTO DEL SISTEMA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Variables del sistema:
; Entradas: imc, edad, colesterol
; Salida: riesgo

(deffacts variables_difusas
   (variable imc)
   (variable edad)
   (variable colesterol)
   (variable riesgo)
)

; Conjuntos difusos trapezoidales.
(deffacts conjuntos_difusos
   ; IMC
   (cd imc normal 0 0 25 27)
   (cd imc elevado 25 30 35 40)
   (cd imc muy_elevado 35 40 100 100)

   ; Edad
   (cd edad nino 0 0 12 16)
   (cd edad joven 14 18 30 35)
   (cd edad adulto 30 35 55 65)
   (cd edad avanzada 55 70 100 100)

   ; Colesterol total
   (cd colesterol normal 0 0 180 200)
   (cd colesterol alto 180 220 240 260)
   (cd colesterol muy_alto 240 280 500 500)

   ; Riesgo de infarto, variable de salida entre 0 y 10
   (cd riesgo bajo 0 0 3 3)
   (cd riesgo medio 4 4 6 6)
   (cd riesgo alto 7 7 8 8)
   (cd riesgo muy_alto 9 9 10 10)
)

; Reglas del sistema.
(deffacts reglas
   ; R1
   (regla R1 antecedente imc elevado)
   (regla R1 antecedente colesterol normal)
   (regla R1 consecuente riesgo medio)
   (regla R1 explicacion "R1: Si el IMC es elevado pero tiene el colesterol normal, el riesgo de infarto es medio")

   ; R2
   (regla R2 antecedente imc elevado)
   (regla R2 antecedente edad avanzada)
   (regla R2 consecuente riesgo alto)
   (regla R2 explicacion "R2: Si el IMC es elevado y la edad es avanzada, el riesgo de infarto es alto")

   ; R3
   (regla R3 antecedente imc muy_elevado)
   (regla R3 consecuente riesgo muy_alto)
   (regla R3 explicacion "R3: Si el IMC es muy elevado, el riesgo de infarto es muy alto")

   ; R4
   (regla R4 antecedente colesterol alto)
   (regla R4 consecuente riesgo alto)
   (regla R4 explicacion "R4: Si el colesterol es alto, el riesgo de infarto es alto")

   ; R5
   (regla R5 antecedente colesterol muy_alto)
   (regla R5 consecuente riesgo muy_alto)
   (regla R5 explicacion "R5: Si el colesterol es muy alto, el riesgo de infarto es muy alto")
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; INTERACCION CON EL USUARIO
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defrule preguntar_datos
   (declare (salience 100))
   =>
   (printout t crlf "...Ejecutando el programa..." crlf)

   (printout t "Indique el IMC: ")
   (bind ?imc (read))
   (assert (dato imc ?imc))

   (printout t "Indique la edad: ")
   (bind ?edad (read))
   (assert (dato edad ?edad))

   (printout t "Indique el colesterol total: ")
   (bind ?colesterol (read))
   (assert (dato colesterol ?colesterol))

   (printout t crlf)
   (assert (modulo calculo_fuzzy))
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; INICIALIZACION DEL MODULO DIFUSO
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defrule iniciar_proceso
   (declare (salience 5))
   (modulo calculo_fuzzy)
   =>
   (assert (borrar_datos_ejecucion_anterior))
)

(defrule borrar_datos_ejecucion_anterior
   (declare (salience 5))
   (modulo calculo_fuzzy)
   (borrar_datos_ejecucion_anterior)
   ?f <- (fuzzy $?)
   =>
   (retract ?f)
)

(defrule borrar_borrar_datos
   (declare (salience 5))
   (modulo calculo_fuzzy)
   ?f <- (borrar_datos_ejecucion_anterior)
   (not (fuzzy $?))
   =>
   (retract ?f)
)

(defrule inicializar_fuzzy_inference
   (declare (salience 4))
   (modulo calculo_fuzzy)
   (regla ?r consecuente ?v ?l)
   (not (fuzzy numerador ?v ?))
   (not (fuzzy denominador ?v ?))
   =>
   (assert (fuzzy numerador ?v 0))
   (assert (fuzzy denominador ?v 0))
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; CALCULO DE GRADOS DE PERTENENCIA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defrule cumplimiento_predicado_difuso
   (declare (salience 3))
   (modulo calculo_fuzzy)
   (cd ?v ?l ?a ?b ?c ?d)
   (dato ?v ?x)
   =>
   (bind ?g (membership ?x ?a ?b ?c ?d))
   (assert (fuzzy cumplimiento ?v ?l ?g))
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; MATCHING DE ANTECEDENTES DE REGLAS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defrule matching_antecedente_simple
   (declare (salience 2))
   (modulo calculo_fuzzy)
   (regla ?r antecedente ?v ?l)
   (fuzzy cumplimiento ?v ?l ?g)
   =>
   (assert (fuzzy matching ?r ?g ?v))
)

(defrule matching_antecedente_1
   (declare (salience 2))
   (modulo calculo_fuzzy)
   ?f <- (fuzzy matching ?r ?g ?v)
   (not (fuzzy matching_antecedente_regla ?r ?))
   =>
   (assert (fuzzy matching_antecedente_regla ?r ?g))
   (retract ?f)
)

(defrule matching_antecedente
   (declare (salience 2))
   (modulo calculo_fuzzy)
   ?f <- (fuzzy matching ?r ?g ?v)
   ?h <- (fuzzy matching_antecedente_regla ?r ?g1)
   =>
   (retract ?f ?h)
   (assert (fuzzy matching_antecedente_regla ?r (conjuncion ?g1 ?g)))
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; INFERENCIA DIFUSA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defrule inferencia_difusa
   (declare (salience 1))
   (modulo calculo_fuzzy)
   (fuzzy matching_antecedente_regla ?r ?g1)
   (test (> ?g1 0))
   (regla ?r consecuente ?v ?l)
   (regla ?r explicacion ?text)
   =>
   (assert (fuzzy inferido ?v ?l ?g1))
   (printout t "Se va a aplicar la regla: " ?text crlf)
   (printout t "Con grado de cumplimiento " (formatear-decimal ?g1) crlf crlf)
)

(defrule inferencia_por_defecto_riesgo_bajo
   (declare (salience 0))
   (modulo calculo_fuzzy)
   (not (fuzzy inferido riesgo ? ?))
   =>
   (assert (fuzzy inferido riesgo bajo 1))
   (printout t "Se va a aplicar la regla: R6: En otro caso, el riesgo de infarto es bajo" crlf)
   (printout t "Con grado de cumplimiento 1.00" crlf crlf)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; DEFUZZIFICACION POR COS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defrule concrecion_individual
   (declare (salience 0))
   (modulo calculo_fuzzy)
   (metodo defuzzificacion COS)
   (fuzzy inferido ?v ?l ?g1)
   (cd ?v ?l ?a ?b ?c ?d)
   =>
   (assert (fuzzy sumando_numerador ?v (* ?g1 (center_of_gravity ?a ?b ?c ?d))))
   (assert (fuzzy sumando_denominador ?v ?g1))
)

(defrule concrecion_numerador
   (declare (salience -1))
   (modulo calculo_fuzzy)
   (metodo defuzzificacion COS)
   ?g <- (fuzzy numerador ?v ?x)
   ?f <- (fuzzy sumando_numerador ?v ?y)
   =>
   (assert (fuzzy numerador ?v (+ ?x ?y)))
   (retract ?f ?g)
)

(defrule concrecion_denominador
   (declare (salience -1))
   (modulo calculo_fuzzy)
   (metodo defuzzificacion COS)
   ?g <- (fuzzy denominador ?v ?x)
   ?f <- (fuzzy sumando_denominador ?v ?y)
   =>
   (assert (fuzzy denominador ?v (+ ?x ?y)))
   (retract ?f ?g)
)

(defrule respuesta_COS
   (declare (salience -2))
   (modulo calculo_fuzzy)
   (metodo defuzzificacion COS)
   (fuzzy numerador riesgo ?n)
   (fuzzy denominador riesgo ?d)
   (test (> ?d 0))
   (not (fuzzy sumando_numerador riesgo ?))
   (not (fuzzy sumando_denominador riesgo ?))
   (not (resultado_impreso))
   =>
   (assert (resultado_impreso))
   (bind ?riesgo (/ ?n ?d))
   (bind ?riesgo_mostrar (formatear-decimal ?riesgo))
   (printout t "Aplicando esta(s) regla(s), el valor de riesgo es " ?riesgo_mostrar crlf)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; SALIDA DEL MODULO
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defrule salir
   (declare (salience -10))
   ?f <- (modulo calculo_fuzzy)
   (resultado_impreso)
   =>
   (retract ?f)
)