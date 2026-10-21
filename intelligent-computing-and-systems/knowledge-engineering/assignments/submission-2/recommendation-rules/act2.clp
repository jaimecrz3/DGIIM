; -----------------------------------------------------------------------------
; act2.clp
; Autores: Jaime Corzo Galdó y Mario Líndez Martínez
; Ingenieria del Conocimiento - Practica 5 - Actividad 2
; Sistema basado en conocimiento con factores de certeza para recomendar coche.
; -----------------------------------------------------------------------------

; -----------------------------------------------------------------------------
; Representacion
; -----------------------------------------------------------------------------
(deftemplate Evidencia
   (slot nombre)
   (slot valor))

(deftemplate FactorCerteza
   (slot hipotesis)
   (slot valor)
   (slot fc))

(deftemplate coche
   (slot id)
   (slot nombre))

(deftemplate Recomendacion
   (slot coche)
   (slot fc)
   (multislot explicacion))

(deffacts coches_disponibles
   (coche (id diesel) (nombre "coche diesel"))
   (coche (id gasolina) (nombre "coche de gasolina"))
   (coche (id hibrido_autorecargable) (nombre "coche hibrido auto-recargable"))
   (coche (id gas) (nombre "coche de GAS"))
   (coche (id hibrido) (nombre "coche hibrido"))
   (coche (id electrico) (nombre "coche electrico")))

; -----------------------------------------------------------------------------
; Funciones de factores de certeza
; -----------------------------------------------------------------------------

(deffunction encadenado (?fc_antecedente ?fc_regla)
   (if (> ?fc_antecedente 0)
      then
         (bind ?rv (* ?fc_antecedente ?fc_regla))
      else
         (bind ?rv 0))
   ?rv)

(deffunction combinacion (?fc1 ?fc2)
   (if (and (> ?fc1 0) (> ?fc2 0))
      then
         (bind ?rv (- (+ ?fc1 ?fc2) (* ?fc1 ?fc2)))
      else
         (if (and (< ?fc1 0) (< ?fc2 0))
            then
               (bind ?rv (+ (+ ?fc1 ?fc2) (* ?fc1 ?fc2)))
            else
               (bind ?rv (/ (+ ?fc1 ?fc2) (- 1 (min (abs ?fc1) (abs ?fc2)))))))
   ?rv)

; Nueva función que transforma el float a texto manualmente saltándose el bug regional de CLIPS
(deffunction formatear-fc (?fc)
   (bind ?v (round (* ?fc 100)))
   (if (= ?v 100) then (return "1.0"))
   (if (= ?v -100) then (return "-1.0"))
   (if (= ?v 0) then (return "0.0"))
   (bind ?signo "")
   (if (< ?v 0)
      then
         (bind ?signo "-")
         (bind ?v (abs ?v)))
   (if (< ?v 10)
      then (return (str-cat ?signo "0.0" ?v))
      else (return (str-cat ?signo "0." ?v)))
)

; -----------------------------------------------------------------------------
; Interaccion con el usuario
; -----------------------------------------------------------------------------

(defrule inicio
   (declare (salience 100))
   =>
   (printout t crlf "...Ejecutando el programa..." crlf)

   (printout t "Cual es tu presupuesto? bajo/medio/alto: ")
   (bind ?presupuesto (read))
   (assert (Evidencia (nombre presupuesto) (valor ?presupuesto)))

   (printout t "Haces menos de 20000 km al anio? si/no: ")
   (bind ?km (read))
   (assert (Evidencia (nombre menos_20000km) (valor ?km)))

   (printout t "Haces muchos trayectos largos? si/no: ")
   (bind ?largos (read))
   (assert (Evidencia (nombre trayectos_largos) (valor ?largos)))

   (printout t "Vas a remolcar un remolque con mucho peso? si/no: ")
   (bind ?remolque (read))
   (assert (Evidencia (nombre remolque_mucho_peso) (valor ?remolque)))

   (printout t "Circulas fundamentalmente por ciudad? si/no: ")
   (bind ?ciudad (read))
   (assert (Evidencia (nombre ciudad) (valor ?ciudad)))

   (printout t "Vas a viajar a ciudades con ZBE? si/no: ")
   (bind ?zbe (read))
   (assert (Evidencia (nombre zbe) (valor ?zbe)))

   (printout t "Tu ciudad tiene beneficios para vehiculos con etiqueta Cero? si/no: ")
   (bind ?cero (read))
   (assert (Evidencia (nombre beneficios_etiqueta_cero) (valor ?cero))))

(defrule certeza_evidencias
   (declare (salience 50))
   (Evidencia (nombre ?e) (valor ?v))
   =>
   (assert (FactorCerteza (hipotesis ?e) (valor ?v) (fc 1))))

; -----------------------------------------------------------------------------
; Reglas del sistema
; -----------------------------------------------------------------------------

; R1. Si haces menos de 20000 km/anio, se aconseja gasolina o hibrido auto-recargable.
(defrule R1_gasolina
   (declare (salience 10))
   (FactorCerteza (hipotesis menos_20000km) (valor si) (fc ?f&:(> ?f 0)))
   =>
   (bind ?fc (encadenado ?f (/ 6 10)))
   (assert (Recomendacion (coche gasolina) (fc ?fc)
      (explicacion (str-cat "R1: haces menos de 20000 km al anio, por eso se aconseja gasolina con FC " (formatear-fc ?fc))))))

(defrule R1_hibrido_autorecargable
   (declare (salience 10))
   (FactorCerteza (hipotesis menos_20000km) (valor si) (fc ?f&:(> ?f 0)))
   =>
   (bind ?fc (encadenado ?f (/ 8 10)))
   (assert (Recomendacion (coche hibrido_autorecargable) (fc ?fc)
      (explicacion (str-cat "R1: haces menos de 20000 km al anio, por eso se aconseja hibrido auto-recargable con FC " (formatear-fc ?fc))))))

; R2. Si se va a remolcar un remolque con mucho peso, se aconseja diesel.
(defrule R2_diesel
   (declare (salience 10))
   (FactorCerteza (hipotesis remolque_mucho_peso) (valor si) (fc ?f&:(> ?f 0)))
   =>
   (bind ?fc (encadenado ?f (/ 5 10)))
   (assert (Recomendacion (coche diesel) (fc ?fc)
      (explicacion (str-cat "R2: vas a remolcar mucho peso, por eso se aconseja diesel con FC " (formatear-fc ?fc))))))

; R3. Si haces muchos trayectos largos, no se aconseja electrico ni GAS.
(defrule R3_electrico
   (declare (salience 10))
   (FactorCerteza (hipotesis trayectos_largos) (valor si) (fc ?f&:(> ?f 0)))
   =>
   (bind ?fc (encadenado ?f (/ -9 10)))
   (assert (Recomendacion (coche electrico) (fc ?fc)
      (explicacion (str-cat "R3: haces muchos trayectos largos, por eso se desaconseja electrico con FC " (formatear-fc ?fc))))))

(defrule R3_gas
   (declare (salience 10))
   (FactorCerteza (hipotesis trayectos_largos) (valor si) (fc ?f&:(> ?f 0)))
   =>
   (bind ?fc (encadenado ?f (/ -3 10)))
   (assert (Recomendacion (coche gas) (fc ?fc)
      (explicacion (str-cat "R3: haces muchos trayectos largos, por eso se desaconseja GAS con FC " (formatear-fc ?fc))))))

; R4. Si el presupuesto es bajo, se aconseja gasolina o GAS.
(defrule R4_gasolina
   (declare (salience 10))
   (FactorCerteza (hipotesis presupuesto) (valor bajo) (fc ?f&:(> ?f 0)))
   =>
   (bind ?fc (encadenado ?f (/ 7 10)))
   (assert (Recomendacion (coche gasolina) (fc ?fc)
      (explicacion (str-cat "R4: tienes presupuesto bajo, por eso se aconseja gasolina con FC " (formatear-fc ?fc))))))

(defrule R4_gas
   (declare (salience 10))
   (FactorCerteza (hipotesis presupuesto) (valor bajo) (fc ?f&:(> ?f 0)))
   =>
   (bind ?fc (encadenado ?f (/ 6 10)))
   (assert (Recomendacion (coche gas) (fc ?fc)
      (explicacion (str-cat "R4: tienes presupuesto bajo, por eso se aconseja GAS con FC " (formatear-fc ?fc))))))

; R5. Si el presupuesto es alto, se aconseja hibrido o electrico.
(defrule R5_hibrido
   (declare (salience 10))
   (FactorCerteza (hipotesis presupuesto) (valor alto) (fc ?f&:(> ?f 0)))
   =>
   (bind ?fc (encadenado ?f (/ 6 10)))
   (assert (Recomendacion (coche hibrido) (fc ?fc)
      (explicacion (str-cat "R5: tienes presupuesto alto, por eso se aconseja hibrido con FC " (formatear-fc ?fc))))))

(defrule R5_electrico
   (declare (salience 10))
   (FactorCerteza (hipotesis presupuesto) (valor alto) (fc ?f&:(> ?f 0)))
   =>
   (bind ?fc (encadenado ?f (/ 7 10)))
   (assert (Recomendacion (coche electrico) (fc ?fc)
      (explicacion (str-cat "R5: tienes presupuesto alto, por eso se aconseja electrico con FC " (formatear-fc ?fc))))))

; R6. Si circulas fundamentalmente por ciudad, se aconseja electrico.
(defrule R6_electrico
   (declare (salience 10))
   (FactorCerteza (hipotesis ciudad) (valor si) (fc ?f&:(> ?f 0)))
   =>
   (bind ?fc (encadenado ?f (/ 8 10)))
   (assert (Recomendacion (coche electrico) (fc ?fc)
      (explicacion (str-cat "R6: circulas fundamentalmente por ciudad, por eso se aconseja electrico con FC " (formatear-fc ?fc))))))

; R7. Si vas a ciudades con ZBE, no se aconseja gasolina ni hibrido auto-recargable.
(defrule R7_gasolina
   (declare (salience 10))
   (FactorCerteza (hipotesis zbe) (valor si) (fc ?f&:(> ?f 0)))
   =>
   (bind ?fc (encadenado ?f (/ -8 10)))
   (assert (Recomendacion (coche gasolina) (fc ?fc)
      (explicacion (str-cat "R7: viajas a ciudades con ZBE, por eso se desaconseja gasolina con FC " (formatear-fc ?fc))))))

(defrule R7_hibrido_autorecargable
   (declare (salience 10))
   (FactorCerteza (hipotesis zbe) (valor si) (fc ?f&:(> ?f 0)))
   =>
   (bind ?fc (encadenado ?f (/ -3 10)))
   (assert (Recomendacion (coche hibrido_autorecargable) (fc ?fc)
      (explicacion (str-cat "R7: viajas a ciudades con ZBE, por eso se desaconseja hibrido auto-recargable con FC " (formatear-fc ?fc))))))

; R8. Si tu ciudad tiene beneficios para etiqueta Cero, se aconseja electrico.
(defrule R8_electrico
   (declare (salience 10))
   (FactorCerteza (hipotesis beneficios_etiqueta_cero) (valor si) (fc ?f&:(> ?f 0)))
   =>
   (bind ?fc (encadenado ?f (/ 2 10)))
   (assert (Recomendacion (coche electrico) (fc ?fc)
      (explicacion (str-cat "R8: tu ciudad tiene beneficios para etiqueta Cero, por eso se aconseja electrico con FC " (formatear-fc ?fc))))))

; -----------------------------------------------------------------------------
; Combinacion de recomendaciones para el mismo tipo de coche
; -----------------------------------------------------------------------------

(defrule combinar_recomendaciones
   (declare (salience 5))
   ?f <- (Recomendacion (coche ?c) (fc ?fc1) (explicacion $?exp1))
   ?g <- (Recomendacion (coche ?c) (fc ?fc2) (explicacion $?exp2))
   (test (neq ?f ?g))
   =>
   (retract ?f ?g)
   (bind ?nuevo (combinacion ?fc1 ?fc2))
   (assert (Recomendacion (coche ?c) (fc ?nuevo) (explicacion $?exp1 $?exp2))))

; -----------------------------------------------------------------------------
; Seleccion de la recomendacion con mayor factor de certeza
; -----------------------------------------------------------------------------

(defrule imprimir_recomendacion
   (declare (salience -10))
   (coche (id ?c) (nombre ?nombre))
   (Recomendacion (coche ?c) (fc ?fc) (explicacion $?exp))
   (not (and
      (coche (id ?otro))
      (Recomendacion (coche ?otro) (fc ?fc2&:(> ?fc2 ?fc)))))
   (not (resultado_impreso))
   =>
   (assert (resultado_impreso))
   (bind ?fc_mostrar (formatear-fc ?fc))
   (printout t crlf)
   (if (> ?fc 0)
      then
         (printout t "Recomendacion: " ?nombre " con factor de certeza: " ?fc_mostrar crlf)
      else
         (printout t "No se obtiene una recomendacion positiva. La opcion menos desaconsejada es " ?nombre " con factor de certeza: " ?fc_mostrar crlf))
   (printout t crlf "Explicacion:" crlf)
   (progn$ (?e $?exp)
      (printout t " - " ?e crlf))
)

(defrule imprimir_sin_recomendacion
   (declare (salience -20))
   (not (resultado_impreso))
   (not (Recomendacion (coche ?c)))
   =>
   (assert (resultado_impreso))
   (printout t crlf "No se ha podido obtener una recomendacion porque no se ha activado ninguna regla." crlf)
   (printout t "Revise las respuestas introducidas. Use bajo/medio/alto para presupuesto y si/no para el resto." crlf))