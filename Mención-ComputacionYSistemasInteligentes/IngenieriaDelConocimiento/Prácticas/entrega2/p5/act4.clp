; -----------------------------------------------------------------------------
; act4.clp
; Autores: Jaime Corzo Galdo y Mario Lindez Marti
; Ingenieria del Conocimiento - Practica 5 - Actividad 4
; Razonamiento probabilistico: probabilidad de que guste comer pasta
; -----------------------------------------------------------------------------

; -----------------------------------------------------------------------------
; Representacion
; -----------------------------------------------------------------------------
; (prob ?variable ?valor ?p)
; (probcond2 gusta_pasta si edad ?e comio_ayer ?c ?p)
; (probcond ?evidencia ?valor gusta_pasta ?g ?p)
; (valor ?variable ?valor)
; -----------------------------------------------------------------------------

(deffacts acumuladores
   (acumulado positivo 0)
   (acumulado negativo 0)
)

; -----------------------------------------------------------------------------
; REGLA DE INICIALIZACIÓN: Evita el bug regional cargando los datos mediante divisiones
; -----------------------------------------------------------------------------
(defrule cargar_probabilidades
   (declare (salience 200))
   =>
   ; P(edad)
   (assert (prob edad joven (/ 35 100)))
   (assert (prob edad mediana (/ 40 100)))
   (assert (prob edad mayor (/ 25 100)))

   ; P(comio_ayer)
   (assert (prob comio_ayer si (/ 30 100)))
   (assert (prob comio_ayer no (/ 70 100)))

   ; P(gusta_pasta = si | edad, comio_ayer)
   (assert (probcond2 gusta_pasta si edad joven comio_ayer si (/ 75 100)))
   (assert (probcond2 gusta_pasta si edad joven comio_ayer no (/ 65 100)))
   (assert (probcond2 gusta_pasta si edad mediana comio_ayer si (/ 70 100)))
   (assert (probcond2 gusta_pasta si edad mediana comio_ayer no (/ 55 100)))
   (assert (probcond2 gusta_pasta si edad mayor comio_ayer si (/ 55 100)))
   (assert (probcond2 gusta_pasta si edad mayor comio_ayer no (/ 45 100)))

   ; P(restaurantes_italianos | gusta_pasta)
   (assert (probcond restaurantes_italianos si gusta_pasta si (/ 85 100)))
   (assert (probcond restaurantes_italianos no gusta_pasta si (/ 15 100)))
   (assert (probcond restaurantes_italianos si gusta_pasta no (/ 20 100)))
   (assert (probcond restaurantes_italianos no gusta_pasta no (/ 80 100)))

   ; P(frecuencia_pasta | gusta_pasta)
   (assert (probcond frecuencia_pasta esporadicamente gusta_pasta si (/ 15 100)))
   (assert (probcond frecuencia_pasta habitualmente gusta_pasta si (/ 45 100)))
   (assert (probcond frecuencia_pasta frecuentemente gusta_pasta si (/ 40 100)))
   (assert (probcond frecuencia_pasta esporadicamente gusta_pasta no (/ 70 100)))
   (assert (probcond frecuencia_pasta habitualmente gusta_pasta no (/ 25 100)))
   (assert (probcond frecuencia_pasta frecuentemente gusta_pasta no (/ 5 100)))

   ; Evidencias desconocidas
   (assert (probcond restaurantes_italianos desconocido gusta_pasta si 1))
   (assert (probcond restaurantes_italianos desconocido gusta_pasta no 1))
   (assert (probcond frecuencia_pasta desconocido gusta_pasta si 1))
   (assert (probcond frecuencia_pasta desconocido gusta_pasta no 1))
)

; -----------------------------------------------------------------------------
; Funcion auxiliar para formatear a 4 decimales sin sufrir el bug de la coma
; -----------------------------------------------------------------------------
(deffunction formatear-4decimales (?x)
   (if (= ?x 0) then (return "0.0000"))
   (bind ?signo "")
   (if (< ?x 0) then
      (bind ?signo "-")
      (bind ?x (abs ?x)))
   (bind ?total (round (* ?x 10000)))
   (bind ?entero (div ?total 10000))
   (bind ?dec (mod ?total 10000))
   (if (< ?dec 10) then (return (str-cat ?signo ?entero ".000" ?dec)))
   (if (< ?dec 100) then (return (str-cat ?signo ?entero ".00" ?dec)))
   (if (< ?dec 1000) then (return (str-cat ?signo ?entero ".0" ?dec)))
   (return (str-cat ?signo ?entero "." ?dec))
)

; -----------------------------------------------------------------------------
; Interaccion con el usuario
; -----------------------------------------------------------------------------

(defrule inicio
   (declare (salience 100))
   =>
   (printout t crlf "Sistema probabilistico para calcular si a una persona le gustaria comer pasta" crlf)
   (printout t "Responda usando las opciones indicadas." crlf crlf)

   (printout t "Edad: 1=joven, 2=mediana, 3=mayor, 4=desconocido: ")
   (bind ?edad (read))
   (if (= ?edad 1)
      then (assert (valor edad joven))
      else
         (if (= ?edad 2)
            then (assert (valor edad mediana))
            else
               (if (= ?edad 3)
                  then (assert (valor edad mayor))
                  else (assert (valor edad desconocido)))))

   (printout t "Comio pasta ayer? 1=si, 2=no, 3=desconocido: ")
   (bind ?comio (read))
   (if (= ?comio 1)
      then (assert (valor comio_ayer si))
      else
         (if (= ?comio 2)
            then (assert (valor comio_ayer no))
            else (assert (valor comio_ayer desconocido))))

   (printout t "Le gustan los restaurantes italianos? 1=si, 2=no, 3=desconocido: ")
   (bind ?rest (read))
   (if (= ?rest 1)
      then (assert (valor restaurantes_italianos si))
      else
         (if (= ?rest 2)
            then (assert (valor restaurantes_italianos no))
            else (assert (valor restaurantes_italianos desconocido))))

   (printout t "Frecuencia con la que come pasta: 1=esporadicamente, 2=habitualmente, 3=frecuentemente, 4=desconocido: ")
   (bind ?freq (read))
   (if (= ?freq 1)
      then (assert (valor frecuencia_pasta esporadicamente))
      else
         (if (= ?freq 2)
            then (assert (valor frecuencia_pasta habitualmente))
            else
               (if (= ?freq 3)
                  then (assert (valor frecuencia_pasta frecuentemente))
                  else (assert (valor frecuencia_pasta desconocido)))))

   (assert (calcular_probabilidad))
)

; -----------------------------------------------------------------------------
; Calculo de aportaciones
; -----------------------------------------------------------------------------

(defrule calcular_aportes
   (declare (salience 50))

   (calcular_probabilidad)

   (valor edad ?edad_usuario)
   (valor comio_ayer ?comio_usuario)
   (valor restaurantes_italianos ?rest_usuario)
   (valor frecuencia_pasta ?freq_usuario)

   (prob edad ?edad ?p_edad)
   (prob comio_ayer ?comio ?p_comio)

   (test (or (eq ?edad_usuario desconocido) (eq ?edad_usuario ?edad)))
   (test (or (eq ?comio_usuario desconocido) (eq ?comio_usuario ?comio)))

   (not (aporte_calculado ?edad ?comio))

   (probcond2 gusta_pasta si edad ?edad comio_ayer ?comio ?p_gusta_causas)

   (probcond restaurantes_italianos ?rest_usuario gusta_pasta si ?p_rest_si)
   (probcond restaurantes_italianos ?rest_usuario gusta_pasta no ?p_rest_no)

   (probcond frecuencia_pasta ?freq_usuario gusta_pasta si ?p_freq_si)
   (probcond frecuencia_pasta ?freq_usuario gusta_pasta no ?p_freq_no)

   =>
   (bind ?aportacion_positiva
      (* ?p_edad ?p_comio ?p_gusta_causas ?p_rest_si ?p_freq_si))

   (bind ?aportacion_negativa
      (* ?p_edad ?p_comio (- 1 ?p_gusta_causas) ?p_rest_no ?p_freq_no))

   (assert (sumar positivo ?aportacion_positiva))
   (assert (sumar negativo ?aportacion_negativa))
   (assert (aporte_calculado ?edad ?comio))
)

(defrule sumar_positivo
   (declare (salience 40))
   ?a <- (acumulado positivo ?x)
   ?s <- (sumar positivo ?y)
   =>
   (retract ?a ?s)
   (assert (acumulado positivo (+ ?x ?y)))
)

(defrule sumar_negativo
   (declare (salience 40))
   ?a <- (acumulado negativo ?x)
   ?s <- (sumar negativo ?y)
   =>
   (retract ?a ?s)
   (assert (acumulado negativo (+ ?x ?y)))
)

; -----------------------------------------------------------------------------
; Resultado final
; -----------------------------------------------------------------------------

(defrule mostrar_resultado
   (declare (salience -10))
   (calcular_probabilidad)
   (acumulado positivo ?pos)
   (acumulado negativo ?neg)
   (not (sumar positivo ?))
   (not (sumar negativo ?))
   (not (resultado_impreso))
   =>
   (bind ?total (+ ?pos ?neg))
   (bind ?p_gusta (/ ?pos ?total))
   (bind ?p_no_gusta (- 1 ?p_gusta))

   (assert (resultado_impreso))

   (printout t crlf "Resultado del razonamiento probabilistico:" crlf)
   (printout t "Probabilidad de que le guste comer pasta: " (formatear-4decimales ?p_gusta) crlf)
   (printout t "Probabilidad de que no le guste comer pasta: " (formatear-4decimales ?p_no_gusta) crlf)
)