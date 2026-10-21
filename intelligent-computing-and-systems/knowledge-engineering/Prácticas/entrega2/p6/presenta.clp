; AUTOR: JAIME CORZO GALDO, MARIO LINDEZ MARTINEZ

(defmodule PRESENTA
    (import MAIN deftemplate preferencia)
    (import MAIN deftemplate modulo)
    (import MAIN deftemplate estado-menu)
    (import MAIN deftemplate ingrediente-disponible)
    (import FILTRA deftemplate candidata)
    (import FILTRA deftemplate receta-descartada)
    (import FILTRA deftemplate proceso-filtrado)
    (import FILTRA deftemplate bonus-aplicado)
    (import MAIN deftemplate receta)
    (import DEDUCCION deftemplate propiedad_receta)
    (import PROPUESTA-TIPO-RECETA deftemplate propuesta-inicial)
    (import INCERTIDUMBRE deftemplate factor-certeza-receta)
    (import INCERTIDUMBRE deftemplate evidencia-incertidumbre)
    (import INCERTIDUMBRE deftemplate proceso-incertidumbre)
)

(deffunction PRESENTA::formatear-fc (?fc)
   (bind ?signo "")
   (if (< ?fc 0)
      then
         (bind ?signo "-")
         (bind ?fc (abs ?fc)))

   ; Convertimos el float a un entero de dos decimales (ej: 0.76 -> 76)
   ; Al forzar el tipo 'integer', destruimos cualquier coma regional latente
   (bind ?total-centenas (integer (round (* ?fc 100))))

   ; Extraemos la parte entera y la parte decimal mediante divisiones enteras
   (bind ?entera (div ?total-centenas 100))
   (bind ?decimal (mod ?total-centenas 100))

   ; Nos aseguramos de que el decimal siempre tenga dos dígitos (ej: 5 -> "05")
   (bind ?str-dec (str-cat ?decimal))
   (if (< ?decimal 10)
      then (bind ?str-dec (str-cat "0" ?decimal)))

   ; Devolvemos la cadena perfectamente construida con un punto limpio
   (return (str-cat ?signo ?entera "." ?str-dec))
)

(deftemplate receta-mostrada
   (slot receta))

(deftemplate receta-actual
   (slot receta))

(deftemplate respuesta-presentacion
   (slot valor))

(deftemplate presentacion-iniciada
   (slot valor))

(defrule PRESENTA::inicializar-presentacion
   (declare (salience 100))
   (modulo (nombre presenta))
   (not (presentacion-iniciada (valor si)))
   =>
   (do-for-all-facts
      ((?a receta-actual))
      TRUE
      (retract ?a))
   (do-for-all-facts
      ((?r respuesta-presentacion))
      TRUE
      (retract ?r))
   (assert (presentacion-iniciada (valor si)))
)

; =========================
; SELECCIONAR LA MEJOR RECETA NO MOSTRADA
; Se elige por factor de certeza, no solo por puntuacion clasica.
; =========================

(defrule PRESENTA::seleccionar-receta-actual
   (declare (salience 50))
   (modulo (nombre presenta))
   (not (receta-actual (receta ?)))
   (factor-certeza-receta (receta ?r) (fc ?fc))
   (not (receta-mostrada (receta ?r)))
   ; Comprobamos que no exista otra receta NO MOSTRADA que tenga un FC mayor:
   (not (and (factor-certeza-receta (receta ?r2) (fc ?fc2&:(> ?fc2 ?fc)))
             (not (receta-mostrada (receta ?r2)))))
   =>
   (assert (receta-actual (receta ?r)))
   (assert (receta-mostrada (receta ?r)))
)

; =========================
; MOSTRAR RECETA ACTUAL
; =========================

(defrule PRESENTA::mostrar-receta-actual
   (declare (salience 40))
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (receta (nombre ?r) (tipo-plato ?tp) (dificultad ?d) (tiempo-cocinado ?t))
   (candidata (receta ?r))
   (factor-certeza-receta (receta ?r) (fc ?fc))
   (not (respuesta-presentacion (valor ?)))
   =>
   (printout t crlf "Receta recomendada: " ?r crlf)
   (printout t "Datos principales:" crlf)
   (printout t " - Tipo de plato: " ?tp crlf)
   (printout t " - Dificultad: " ?d crlf)
   (printout t " - Tiempo: " ?t " minutos" crlf)
   (printout t " - Factor de certeza de la recomendacion: " (formatear-fc ?fc) crlf)
   (printout t "Análisis de tus preferencias y restricciones:" crlf)
)

; =============================================================================
; EXPLICACIÓN DE RESTRICCIONES DURAS CUMPLIDAS
; =============================================================================

(defrule PRESENTA::explicar-restriccion-vegana
   (declare (salience 35))
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (preferencia (nombre vegana) (valor si))
   (propiedad_receta es_vegana ?r si)
   (not (respuesta-presentacion (valor ?)))
   =>
   (printout t " -> [Obligatorio] Cumple tu restricción de ser comida VEGANA." crlf)
)

(defrule PRESENTA::explicar-restriccion-vegetariana
   (declare (salience 35))
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (preferencia (nombre vegetariana) (valor si))
   (propiedad_receta es_vegetariana ?r si)
   (not (respuesta-presentacion (valor ?)))
   =>
   (printout t " -> [Obligatorio] Cumple tu restricción de ser VEGETARIANA." crlf)
)

(defrule PRESENTA::explicar-restriccion-sin-gluten
   (declare (salience 35))
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (preferencia (nombre sin-gluten) (valor si))
   (propiedad_receta es_sin_gluten ?r si)
   (not (respuesta-presentacion (valor ?)))
   =>
   (printout t " -> [Obligatorio] Cumple tu restricción: es SIN GLUTEN (apta para celíacos)." crlf)
)

(defrule PRESENTA::explicar-restriccion-sin-lactosa
   (declare (salience 35))
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (preferencia (nombre sin-lactosa) (valor si))
   (propiedad_receta es_sin_lactosa ?r si)
   (not (respuesta-presentacion (valor ?)))
   =>
   (printout t " -> [Obligatorio] Cumple tu restricción: es SIN LACTOSA." crlf)
)

(defrule PRESENTA::explicar-restriccion-tiempo
   (declare (salience 35))
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (preferencia (nombre tiempo-max) (valor ?tmax&:(> ?tmax 0)))
   (receta (nombre ?r) (tiempo-cocinado ?t))
   (not (respuesta-presentacion (valor ?)))
   =>
   (printout t " -> [Obligatorio] El tiempo de cocinado (" ?t " min) no supera tu máximo (" ?tmax " min)." crlf)
)

(defrule PRESENTA::explicar-restriccion-dificultad
   (declare (salience 35))
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (preferencia (nombre dificultad-max) (valor ?dmax&~indiferente))
   (receta (nombre ?r) (dificultad ?d))
   (not (respuesta-presentacion (valor ?)))
   =>
   (printout t " -> [Obligatorio] La dificultad (" ?d ") es aceptable para tu límite (" ?dmax ")." crlf)
)

(defrule PRESENTA::explicar-evidencias
   (declare (salience 30))
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (evidencia-incertidumbre (receta ?r) (descripcion ?d) (valor ?v))
   (not (respuesta-presentacion (valor ?)))
   =>
   ; Aplicamos el formateador aquí también para limpiar los impactos:
   (printout t " -> " ?d " [Impacto: " (formatear-fc ?v) "]" crlf)
)

(defrule PRESENTA::explicar-si-no-hay-evidencias
   (declare (salience 20))
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (not (evidencia-incertidumbre (receta ?r)))
   (not (respuesta-presentacion (valor ?)))
   =>
   (printout t " - No habia preferencias suficientes para aportar evidencias fuertes; se propone una candidata compatible." crlf)
)

; =========================
; PREGUNTAR SIGUIENTE PASO
; =========================

(defrule PRESENTA::preguntar-opcion
   (declare (salience -10))
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (not (respuesta-presentacion (valor ?)))
   =>
   (printout t crlf "¿Que quieres hacer ahora? (mas-similares/mas-restricciones/aceptar): ")
   (assert (respuesta-presentacion (valor (read))))
)

; =========================
; RESPUESTAS DEL USUARIO
; =========================

(defrule PRESENTA::mas-similares
   ?resp <- (respuesta-presentacion (valor mas-similares))
   ?act <- (receta-actual (receta ?r))
   =>
   (retract ?resp)
   (retract ?act)
)

(defrule PRESENTA::aceptar-receta
   ?resp <- (respuesta-presentacion (valor aceptar))
   ?m <- (modulo (nombre presenta))
   =>
   (retract ?resp)
   (retract ?m)

   (do-for-all-facts
      ((?ra receta-actual))
      TRUE
      (retract ?ra))
   (do-for-all-facts
      ((?rm receta-mostrada))
      TRUE
      (retract ?rm))
   (do-for-all-facts
      ((?pi presentacion-iniciada))
      TRUE
      (retract ?pi))
   (do-for-all-facts
      ((?pp propuesta-inicial))
      TRUE
      (retract ?pp))

   (printout t crlf "Perfecto. Espero que te guste la receta elegida." crlf)
)

(defrule PRESENTA::volver-a-restringir
   ?resp <- (respuesta-presentacion (valor mas-restricciones))
   ?m <- (modulo (nombre presenta))
   =>
   (retract ?resp)
   (retract ?m)

   (do-for-all-facts
      ((?c candidata))
      TRUE
      (retract ?c))
   (do-for-all-facts
      ((?d receta-descartada))
      TRUE
      (retract ?d))
   (do-for-all-facts
      ((?rm receta-mostrada))
      TRUE
      (retract ?rm))
   (do-for-all-facts
      ((?ra receta-actual))
      TRUE
      (retract ?ra))
   (do-for-all-facts
      ((?pi presentacion-iniciada))
      TRUE
      (retract ?pi))
   (do-for-all-facts
      ((?pp propuesta-inicial))
      TRUE
      (retract ?pp))
   (do-for-all-facts
      ((?pf proceso-filtrado))
      TRUE
      (retract ?pf))
   (do-for-all-facts
      ((?b bonus-aplicado))
      TRUE
      (retract ?b))
   (do-for-all-facts
      ((?fc factor-certeza-receta))
      TRUE
      (retract ?fc))
   (do-for-all-facts
      ((?ev evidencia-incertidumbre))
      TRUE
      (retract ?ev))
   (do-for-all-facts
      ((?pi2 proceso-incertidumbre))
      TRUE
      (retract ?pi2))

   (assert (modulo (nombre preguntar)))
   (assert (estado-menu (valor activo)))
   (focus MAIN)
)

(defrule PRESENTA::respuesta-no-valida-con-receta-actual
   ?resp <- (respuesta-presentacion (valor ?x&~mas-similares&~mas-restricciones&~aceptar))
   (receta-actual (receta ?))
   =>
   (retract ?resp)
   (printout t "Respuesta no valida. Escribe mas-similares, mas-restricciones o aceptar." crlf)
)

(defrule PRESENTA::respuesta-no-valida-sin-receta-actual
   ?resp <- (respuesta-presentacion (valor ?x&~mas-restricciones&~aceptar))
   (not (receta-actual (receta ?)))
   =>
   (retract ?resp)
   (printout t "Respuesta no valida. Escribe mas-restricciones o aceptar." crlf)
)

; =========================
; SI YA NO QUEDAN MAS RECETAS POR MOSTRAR
; =========================

(defrule PRESENTA::sin-mas-recetas
   (declare (salience -50))
   ?m <- (modulo (nombre presenta))
   (not (receta-actual (receta ?)))
   ; Si no queda ninguna candidata que no haya sido mostrada:
   (not (and (candidata (receta ?r))
             (not (receta-mostrada (receta ?r)))))
   =>
   (printout t crlf "No quedan mas recetas similares para mostrar." crlf)
   (printout t "Puedes aceptar la ultima receta mostrada o pedir mas restricciones." crlf)
   (printout t "¿Que quieres hacer ahora? (mas-restricciones/aceptar): ")
   (assert (respuesta-presentacion (valor (read))))
)


