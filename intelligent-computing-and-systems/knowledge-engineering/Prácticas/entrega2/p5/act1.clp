; ------------------------------------------------------------
; act1.clp
; Autores: Jaime Corzo Galdó y Mario Líndez Martínez
; Practica 5 - Actividad 1
; Logica por defecto: deducibilidad de intereses de prestamos
; ------------------------------------------------------------

(deffacts datos
   ; Prestamos disponibles
   (prestamo privado)
   (prestamo vivienda)
   (prestamo general)

   ; El prestamo privado es personal y no consta que sea para vivienda
   (personal privado)
   (compra_vivienda privado no)

   ; El prestamo vivienda es personal y se emplea para comprar vivienda
   (personal vivienda)
   (compra_vivienda vivienda si)

   ; El prestamo general no es personal
   (no_personal general)
)

; ------------------------------------------------------------
; Funcion auxiliar para preguntar si/no/no_lo_se
; ------------------------------------------------------------

(deffunction preguntar-si-no-ns (?texto)
   (printout t ?texto)
   (bind ?r (read))
   (while (and (neq ?r si) (neq ?r no) (neq ?r no_lo_se)) do
      (printout t "Respuesta no valida. Escriba si, no o no_lo_se: ")
      (bind ?r (read))
   )
   ?r
)

; ------------------------------------------------------------
; Funcion auxiliar para preguntar personal/no_personal/no_lo_se
; ------------------------------------------------------------

(deffunction preguntar-tipo-personal (?texto)
   (printout t ?texto)
   (bind ?r (read))
   (while (and (neq ?r personal) (neq ?r no_personal) (neq ?r no_lo_se)) do
      (printout t "Respuesta no valida. Escriba personal, no_personal o no_lo_se: ")
      (bind ?r (read))
   )
   ?r
)

; ------------------------------------------------------------
; Interaccion inicial con el usuario
; ------------------------------------------------------------

(defrule inicio
   (declare (salience 100))
   =>
   (printout t crlf "...Ejecutando el programa..." crlf)
   (printout t "Prestamos disponibles:" crlf)
   (printout t "1: privado" crlf)
   (printout t "2: vivienda" crlf)
   (printout t "3: general" crlf crlf)

   (printout t "Indique el prestamo en el que esta interesado (del listado u otro): ")
   (bind ?p (read))

   (assert (prestamo_consultado ?p))

   ; Si el prestamo no esta en la base de conocimiento, se pregunta al usuario
   (if (and (neq ?p privado) (neq ?p vivienda) (neq ?p general))
      then
         (assert (prestamo ?p))

         (bind ?tipo (preguntar-tipo-personal
            "Indique si es personal o no_personal o no_lo_se...: "))

         (if (eq ?tipo personal)
            then
               (assert (personal ?p))
            else
               (if (eq ?tipo no_personal)
                  then
                     (assert (no_personal ?p))
               )
         )

         (bind ?viv (preguntar-si-no-ns
            "Indique si el prestamo se emplea para comprar vivienda: si, no o no_lo_se...: "))

         (if (eq ?viv si)
            then
               (assert (compra_vivienda ?p si))
            else
               (if (eq ?viv no)
                  then
                     (assert (compra_vivienda ?p no))
               )
         )
   )
)

; ------------------------------------------------------------
; Regla segura:
; Si un prestamo personal se emplea para comprar vivienda,
; sus intereses son deducibles con certeza segura.
; ------------------------------------------------------------

(defrule prestamo_personal_vivienda_deducible
   (declare (salience 20))
   (prestamo_consultado ?p)
   (prestamo ?p)
   (personal ?p)
   (compra_vivienda ?p si)
   (not (deducible ?p si seguro))
   =>
   (assert (deducible ?p si seguro))
   (assert (explicacion deducible ?p si seguro
      "los intereses son deducibles porque se trata de un prestamo personal empleado para comprar una vivienda"))
)

; ------------------------------------------------------------
; Regla por defecto especifica:
; Los intereses de los prestamos personales no son deducibles,
; salvo que se sepa que se emplean para comprar vivienda.
; ------------------------------------------------------------

(defrule prestamo_personal_no_deducible_por_defecto
   (declare (salience -5))
   (prestamo_consultado ?p)
   (prestamo ?p)
   (personal ?p)
   (not (compra_vivienda ?p si))
   (not (deducible ?p ? ?))
   =>
   (assert (deducible ?p no por_defecto))
   (assert (explicacion deducible ?p no por_defecto
      "asumo que los intereses no son deducibles porque los intereses de los prestamos personales, sin mayor informacion, no son deducibles"))
)

; ------------------------------------------------------------
; Regla por defecto general:
; Los intereses de los prestamos son deducibles.
; Se aplica cuando no se sabe que el prestamo sea personal.
; ------------------------------------------------------------

(defrule prestamo_deducible_por_defecto
   (declare (salience -10))
   (prestamo_consultado ?p)
   (prestamo ?p)
   (not (personal ?p))
   (not (deducible ?p ? ?))
   =>
   (assert (deducible ?p si por_defecto))
   (assert (explicacion deducible ?p si por_defecto
      "asumo que los intereses son deducibles porque, por defecto, los intereses de los prestamos son deducibles"))
)

; ------------------------------------------------------------
; Retraccion:
; Si hay una conclusion segura, se elimina una conclusion por defecto
; sobre el mismo prestamo.
; ------------------------------------------------------------

(defrule retracta_deduccion_por_defecto
   (declare (salience 30))
   ?f <- (deducible ?p ?r por_defecto)
   (deducible ?p ?s seguro)
   =>
   (retract ?f)
)

; ------------------------------------------------------------
; Salida final
; ------------------------------------------------------------

(defrule mostrar_resultado_deducible
   (declare (salience -100))
   (prestamo_consultado ?p)
   (deducible ?p ?r ?certeza)
   (explicacion deducible ?p ?r ?certeza ?expl)
   (not (finalizado))
   =>
   (if (eq ?r si)
      then
         (printout t crlf "Los intereses son deducibles, con certeza " ?certeza crlf)
      else
         (printout t crlf "Los intereses no son deducibles, con certeza " ?certeza crlf)
   )

   (printout t "Explicacion: " ?expl crlf)
   (assert (finalizado))
)

