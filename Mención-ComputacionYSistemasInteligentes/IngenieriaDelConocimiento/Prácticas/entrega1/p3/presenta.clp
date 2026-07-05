; AUTORES: JAIME CORZO GALDO, MARIO LINDEZ MARTINEZ

(defmodule PRESENTA
    (import MAIN deftemplate preferencia)
    (import MAIN deftemplate modulo)
    (import MAIN deftemplate estado-menu)
    (import MAIN deftemplate ingrediente-disponible)
    (import FILTRA deftemplate candidata)
    (import FILTRA deftemplate receta-descartada)
    (import MAIN deftemplate receta)
    (import DEDUCCION deftemplate propiedad_receta)
    (import PROPUESTA-TIPO-RECETA deftemplate propuesta-inicial)
    (import FILTRA deftemplate proceso-filtrado)
    (import FILTRA deftemplate bonus-aplicado)
)

(deftemplate receta-mostrada
   (slot receta))

(deftemplate receta-actual
   (slot receta))

(deftemplate respuesta-presentacion
   (slot valor))

(deftemplate presentacion-iniciada
   (slot valor))

(deffunction PRESENTA::calorias-coinciden (?pref ?real)
   (if (eq ?pref ?real) then
      (return TRUE))
   (if (and (eq ?pref alta) (eq ?real calorica)) then
      (return TRUE))
   (if (and (eq ?pref calorica) (eq ?real alta)) then
      (return TRUE))
   (return FALSE)
)

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
; SELECCIONAR LA MEJOR NO MOSTRADA
; =========================

(defrule PRESENTA::seleccionar-receta-actual
   (declare (salience 50))
   (modulo (nombre presenta))
   (not (receta-actual (receta ?)))
   ?c <- (candidata (receta ?r) (puntuacion ?p))
   (not (receta-mostrada (receta ?r)))
   (not (and
         (candidata (receta ?r2) (puntuacion ?p2))
         (not (receta-mostrada (receta ?r2)))
         (test (> ?p2 ?p))))
   =>
   (assert (receta-actual (receta ?r)))
   (assert (receta-mostrada (receta ?r)))
)


; =========================
; MOSTRAR RECETA ACTUAL
; =========================

(defrule PRESENTA::mostrar-receta-actual
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (receta (nombre ?r) (tipo-plato ?tp) (dificultad ?d) (tiempo-cocinado ?t))
   (candidata (receta ?r) (puntuacion ?p))
   (not (respuesta-presentacion (valor ?)))
   =>
   (printout t crlf "Receta recomendada: " ?r crlf)
   (printout t "Motivos de la recomendacion:" crlf)
   (printout t " - Tipo de plato: " ?tp crlf)
   (printout t " - Dificultad: " ?d crlf)
   (printout t " - Tiempo: " ?t " minutos" crlf)
   (printout t " - Puntuacion total: " ?p crlf)
)

(defrule PRESENTA::explicar-grupo
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (propuesta-inicial (grupo ?g))
   (propiedad_receta grupo_culinario ?r ?g)
   =>
   (printout t " - Coincide con el grupo culinario recomendado: " ?g crlf)
)

(defrule PRESENTA::explicar-calorias
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (preferencia (nombre calorias) (valor ?pref&~indiferente))
   (propiedad_receta calorias ?r ?real)
   (test (calorias-coinciden ?pref ?real))
   =>
   (printout t " - Coincide con el nivel de calorias pedido: " ?pref crlf)
)

(defrule PRESENTA::explicar-picante-si
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (preferencia (nombre picante) (valor si))
   (propiedad_receta es_picante ?r si)
   =>
   (printout t " - Coincide con tu preferencia por recetas picantes." crlf)
)

(defrule PRESENTA::explicar-picante-no
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (preferencia (nombre picante) (valor no))
   (propiedad_receta es_picante ?r no)
   =>
   (printout t " - Coincide con tu preferencia por recetas no picantes." crlf)
)

(defrule PRESENTA::explicar-ingredientes-disponibles
   (modulo (nombre presenta))
   (receta-actual (receta ?r))
   (ingrediente-disponible (nombre ?ing))
   (propiedad_receta ingrediente_relevante ?r ?ing)
   =>
   (printout t " - Aprovecha un ingrediente disponible: " ?ing crlf)
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
   (not (and
         (candidata (receta ?r) (puntuacion ?p))
         (not (receta-mostrada (receta ?r)))))
   =>
   (printout t crlf "No quedan mas recetas similares para mostrar." crlf)
   (printout t "Puedes aceptar la ultima receta mostrada o pedir mas restricciones." crlf)
   (printout t "¿Que quieres hacer ahora? (mas-restricciones/aceptar): ")
   (assert (respuesta-presentacion (valor (read))))
)