; AUTORES: JAIME CORZO GALDO, MARIO LINDEZ MARTINEZ

(defmodule FILTRA
    (import MAIN deftemplate preferencia)
    (import MAIN deftemplate modulo)
    (import MAIN deftemplate ingrediente-disponible)
    (import MAIN deftemplate estado-menu)
    (import PROPUESTA-TIPO-RECETA deftemplate propuesta-inicial)
    (import MAIN deftemplate receta)
    (import DEDUCCION deftemplate propiedad_receta)
    (export ?ALL)
)

(deftemplate candidata
   (slot receta)
   (slot puntuacion (type INTEGER) (default 0)))

(deftemplate receta-descartada
   (slot receta)
   (slot motivo))

(deftemplate proceso-filtrado
   (slot estado))

(deftemplate bonus-aplicado
   (slot receta)
   (slot criterio)
   (slot valor))

(deffunction FILTRA::nivel-dificultad (?d)
   (if (eq ?d facil) then (return 1))
   (if (eq ?d media) then (return 2))
   (if (eq ?d dificil) then (return 3))
   (return 999)
)

(deffunction FILTRA::calorias-coinciden (?pref ?real)
   (if (eq ?pref ?real) then
      (return TRUE))
   (if (and (eq ?pref alta) (eq ?real calorica)) then
      (return TRUE))
   (if (and (eq ?pref calorica) (eq ?real alta)) then
      (return TRUE))
   (return FALSE)
)


(defrule FILTRA::inicializar-filtrado
   (declare (salience 100))
   (modulo (nombre filtrar))
   (not (proceso-filtrado (estado iniciado)))
   =>
   (do-for-all-facts
      ((?c candidata))
      TRUE
      (retract ?c))
   (do-for-all-facts
      ((?d receta-descartada))
      TRUE
      (retract ?d))
   (do-for-all-facts
      ((?p proceso-filtrado))
      TRUE
      (retract ?p))
   (do-for-all-facts
      ((?b bonus-aplicado))
      TRUE
      (retract ?b))
   (assert (proceso-filtrado (estado iniciado)))
)

(defrule FILTRA::crear-candidatas
   (declare (salience 95))
   (modulo (nombre filtrar))
   (proceso-filtrado (estado iniciado))
   (receta (nombre ?r))
   (not (candidata (receta ?r)))
   (not (receta-descartada (receta ?r) (motivo ?)))
   =>
   (assert (candidata (receta ?r) (puntuacion 0)))
)


; =========================
; DESCARTE POR RESTRICCIONES DURAS
; =========================

(defrule FILTRA::descartar-por-tipo-plato
   ?c <- (candidata (receta ?r))
   (preferencia (nombre tipo-plato) (valor ?tp&~cualquiera))
   (receta (nombre ?r) (tipo-plato ?tipo))
   (test (neq ?tp ?tipo))
   =>
   (retract ?c)
   (assert (receta-descartada (receta ?r) (motivo tipo-plato)))
)

(defrule FILTRA::descartar-por-vegana
   ?c <- (candidata (receta ?r))
   (preferencia (nombre vegana) (valor si))
   (propiedad_receta es_vegana ?r no)
   =>
   (retract ?c)
   (assert (receta-descartada (receta ?r) (motivo no-vegana)))
)

(defrule FILTRA::descartar-por-vegetariana
   ?c <- (candidata (receta ?r))
   (preferencia (nombre vegetariana) (valor si))
   (propiedad_receta es_vegetariana ?r no)
   =>
   (retract ?c)
   (assert (receta-descartada (receta ?r) (motivo no-vegetariana)))
)

(defrule FILTRA::descartar-por-sin-gluten
   ?c <- (candidata (receta ?r))
   (preferencia (nombre sin-gluten) (valor si))
   (propiedad_receta es_sin_gluten ?r no)
   =>
   (retract ?c)
   (assert (receta-descartada (receta ?r) (motivo con-gluten)))
)

(defrule FILTRA::descartar-por-sin-lactosa
   ?c <- (candidata (receta ?r))
   (preferencia (nombre sin-lactosa) (valor si))
   (propiedad_receta es_sin_lactosa ?r no)
   =>
   (retract ?c)
   (assert (receta-descartada (receta ?r) (motivo con-lactosa)))
)

(defrule FILTRA::descartar-por-tiempo
   ?c <- (candidata (receta ?r))
   (preferencia (nombre tiempo-max) (valor ?tmax))
   (test (> ?tmax 0))
   (receta (nombre ?r) (tiempo-cocinado ?t))
   (test (> ?t ?tmax))
   =>
   (retract ?c)
   (assert (receta-descartada (receta ?r) (motivo demasiado-lenta)))
)

(defrule FILTRA::descartar-por-dificultad
   ?c <- (candidata (receta ?r))
   (preferencia (nombre dificultad-max) (valor ?dmax&~indiferente))
   (receta (nombre ?r) (dificultad ?d))
   (test (> (nivel-dificultad ?d) (nivel-dificultad ?dmax)))
   =>
   (retract ?c)
   (assert (receta-descartada (receta ?r) (motivo demasiada-dificultad)))
)



; =========================
; PUNTUACION
; =========================

(defrule FILTRA::sumar-por-grupo-propuesto
   ?c <- (candidata (receta ?r) (puntuacion ?p))
   (propuesta-inicial (grupo ?g))
   (propiedad_receta grupo_culinario ?r ?g)
   (not (bonus-aplicado (receta ?r) (criterio grupo-propuesto) (valor ?g)))
   =>
   (modify ?c (puntuacion (+ ?p 5)))
   (assert (bonus-aplicado (receta ?r) (criterio grupo-propuesto) (valor ?g)))
)

(defrule FILTRA::sumar-por-calorias
   ?c <- (candidata (receta ?r) (puntuacion ?p))
   (preferencia (nombre calorias) (valor ?cal&~indiferente))
   (propiedad_receta calorias ?r ?cal-rec)
   (test (calorias-coinciden ?cal ?cal-rec))
   (not (bonus-aplicado (receta ?r) (criterio calorias) (valor ?cal-rec)))
   =>
   (modify ?c (puntuacion (+ ?p 3)))
   (assert (bonus-aplicado (receta ?r) (criterio calorias) (valor ?cal-rec)))
)

(defrule FILTRA::sumar-por-picante-si
   ?c <- (candidata (receta ?r) (puntuacion ?p))
   (preferencia (nombre picante) (valor si))
   (propiedad_receta es_picante ?r si)
   (not (bonus-aplicado (receta ?r) (criterio picante) (valor si)))
   =>
   (modify ?c (puntuacion (+ ?p 2)))
   (assert (bonus-aplicado (receta ?r) (criterio picante) (valor si)))
)

(defrule FILTRA::sumar-por-picante-no
   ?c <- (candidata (receta ?r) (puntuacion ?p))
   (preferencia (nombre picante) (valor no))
   (propiedad_receta es_picante ?r no)
   (not (bonus-aplicado (receta ?r) (criterio picante) (valor no)))
   =>
   (modify ?c (puntuacion (+ ?p 2)))
   (assert (bonus-aplicado (receta ?r) (criterio picante) (valor no)))
)

(defrule FILTRA::sumar-por-ingrediente-disponible
   ?c <- (candidata (receta ?r) (puntuacion ?p))
   (ingrediente-disponible (nombre ?ing))
   (propiedad_receta ingrediente_relevante ?r ?ing)
   (not (bonus-aplicado (receta ?r) (criterio ingrediente-disponible) (valor ?ing)))
   =>
   (modify ?c (puntuacion (+ ?p 1)))
   (assert (bonus-aplicado (receta ?r) (criterio ingrediente-disponible) (valor ?ing)))
)


; =========================
; TRANSICION A PRESENTA
; =========================

(defrule FILTRA::pasar-a-presenta
   (declare (salience -100))
   ?m <- (modulo (nombre filtrar))
   (candidata (receta ?r) (puntuacion ?p))
   =>
   (retract ?m)
   (assert (modulo (nombre presenta)))
   (focus PRESENTA)
)

(defrule FILTRA::sin-candidatas
   (declare (salience -110))
   ?m <- (modulo (nombre filtrar))
   (not (candidata (receta ?)))
   =>
   (printout t crlf "No he encontrado recetas que cumplan todas las restricciones." crlf)
   (retract ?m)
   (assert (modulo (nombre preguntar)))
   (assert (estado-menu (valor activo)))
   (focus MAIN)
)