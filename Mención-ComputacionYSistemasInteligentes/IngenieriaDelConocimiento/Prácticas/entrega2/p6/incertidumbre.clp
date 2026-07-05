; AUTOR: JAIME CORZO GALDO, MARIO LINDEZ MARTINEZ

(defmodule INCERTIDUMBRE
   (import MAIN deftemplate preferencia)
   (import MAIN deftemplate modulo)
   (import MAIN deftemplate ingrediente-disponible)
   (import MAIN deftemplate receta)
   (import FILTRA deftemplate candidata)
   (import DEDUCCION deftemplate propiedad_receta)
   (import PROPUESTA-TIPO-RECETA deftemplate propuesta-inicial)
   (export ?ALL)
)

(deftemplate factor-certeza-receta
   (slot receta)
   (slot fc (type FLOAT) (default 0.0)))

(deftemplate evidencia-incertidumbre
   (slot receta)
   (slot criterio)
   (slot descripcion (type STRING))
   (slot valor (type FLOAT))
   (slot aplicada (allowed-symbols si no) (default no)))

(deftemplate proceso-incertidumbre
   (slot estado))

; -----------------------------------------------------------------------------
; Funciones de combinación
; -----------------------------------------------------------------------------

(deffunction INCERTIDUMBRE::encadenado (?fc-antecedente ?fc-regla)
   (if (> ?fc-antecedente 0)
      then (* ?fc-antecedente ?fc-regla)
      else 0.0))

(deffunction INCERTIDUMBRE::combinacion (?fc1 ?fc2)
   (if (and (> ?fc1 0) (> ?fc2 0))
      then (- (+ ?fc1 ?fc2) (* ?fc1 ?fc2))
      else
         (if (and (< ?fc1 0) (< ?fc2 0))
            then (+ (+ ?fc1 ?fc2) (* ?fc1 ?fc2))
            else (/ (+ ?fc1 ?fc2) (- 1 (min (abs ?fc1) (abs ?fc2)))))))

; -----------------------------------------------------------------------------
; Inicialización
; -----------------------------------------------------------------------------

(defrule INCERTIDUMBRE::inicializar-incertidumbre
   (declare (salience 100))
   (modulo (nombre incertidumbre))
   (not (proceso-incertidumbre (estado iniciado)))
   =>
   (do-for-all-facts ((?f factor-certeza-receta)) TRUE (retract ?f))
   (do-for-all-facts ((?e evidencia-incertidumbre)) TRUE (retract ?e))
   (assert (proceso-incertidumbre (estado iniciado))))

(defrule INCERTIDUMBRE::crear-factor-inicial
   (declare (salience 90))
   (modulo (nombre incertidumbre))
   (proceso-incertidumbre (estado iniciado))
   (candidata (receta ?r))
   (not (factor-certeza-receta (receta ?r)))
   =>
   (assert (factor-certeza-receta (receta ?r) (fc 0.0))))

; -----------------------------------------------------------------------------
; Reglas de Evidencias (Blandas)
; -----------------------------------------------------------------------------

; Grupo culinario coincide
(defrule INCERTIDUMBRE::E1_grupo_culinario_favorable
   (declare (salience 20))
   (factor-certeza-receta (receta ?r))
   (propuesta-inicial (grupo ?g))
   (propiedad_receta grupo_culinario ?r ?g)
   (not (evidencia-incertidumbre (receta ?r) (criterio grupo-culinario)))
   =>
   (assert (evidencia-incertidumbre
      (receta ?r) (criterio grupo-culinario)
      (descripcion (str-cat "Coincide con el grupo culinario propuesto (" ?g ")"))
      (valor (encadenado 1 (/ 6 10)))))) ; <- Cambiado 1.0 por 1 y 0.60 por (/ 6 10)

; Grupo culinario NO coincide (Evidencia negativa)
(defrule INCERTIDUMBRE::E2_grupo_culinario_contrario
   (declare (salience 20))
   (factor-certeza-receta (receta ?r))
   (propuesta-inicial (grupo ?g))
   (not (propiedad_receta grupo_culinario ?r ?g))
   (not (evidencia-incertidumbre (receta ?r) (criterio grupo-culinario)))
   =>
   (assert (evidencia-incertidumbre
      (receta ?r) (criterio grupo-culinario)
      (descripcion (str-cat "No pertenece al grupo culinario recomendado (" ?g ")"))
      (valor (encadenado 1 (/ -3 10)))))) ; <- Cambiado -0.30 por (/ -3 10)

; Calorías coinciden con la preferencia
(defrule INCERTIDUMBRE::E3_calorias_favorables
   (declare (salience 20))
   (factor-certeza-receta (receta ?r))
   (preferencia (nombre calorias) (valor ?cal&~indiferente))
   (propiedad_receta calorias ?r ?cal)
   (not (evidencia-incertidumbre (receta ?r) (criterio calorias)))
   =>
   (assert (evidencia-incertidumbre
      (receta ?r) (criterio calorias)
      (descripcion (str-cat "Tiene exactamente el nivel de calorias preferido: " ?cal))
      (valor (encadenado 1 (/ 5 10))))))

; Calorías NO coinciden con la preferencia
(defrule INCERTIDUMBRE::E4_calorias_contrarias
   (declare (salience 20))
   (factor-certeza-receta (receta ?r))
   (preferencia (nombre calorias) (valor ?cal&~indiferente))
   (propiedad_receta calorias ?r ?cal-real)
   (test (neq ?cal ?cal-real))
   (not (evidencia-incertidumbre (receta ?r) (criterio calorias)))
   =>
   (assert (evidencia-incertidumbre
      (receta ?r) (criterio calorias)
      (descripcion (str-cat "No coincide el nivel de calorias (preferias " ?cal " y es " ?cal-real ")"))
      (valor (encadenado 1 (/ -4 10))))))

; Gusto por el Picante coincide
(defrule INCERTIDUMBRE::E5_picante_coincide
   (declare (salience 20))
   (factor-certeza-receta (receta ?r))
   (preferencia (nombre picante) (valor ?p&~indiferente))
   (propiedad_receta es_picante ?r ?p)
   (not (evidencia-incertidumbre (receta ?r) (criterio picante)))
   =>
   (assert (evidencia-incertidumbre
      (receta ?r) (criterio picante)
      (descripcion (str-cat "Se ajusta a tu preferencia sobre el picante: " ?p))
      (valor (encadenado 1 (/ 4 10))))))

; Gusto por el Picante NO coincide
(defrule INCERTIDUMBRE::E6_picante_no_coincide
   (declare (salience 20))
   (factor-certeza-receta (receta ?r))
   (preferencia (nombre picante) (valor ?p&~indiferente))
   (propiedad_receta es_picante ?r ?p-real)
   (test (neq ?p ?p-real))
   (not (evidencia-incertidumbre (receta ?r) (criterio picante)))
   =>
   (assert (evidencia-incertidumbre
      (receta ?r) (criterio picante)
      (descripcion "Contradice tu gusto sobre el picante")
      (valor (encadenado 1 (/ -5 10))))))

; Aprovechamiento de ingredientes de la nevera
(defrule INCERTIDUMBRE::E7_ingrediente_disponible
   (declare (salience 20))
   (factor-certeza-receta (receta ?r))
   (ingrediente-disponible (nombre ?ing))
   (propiedad_receta ingrediente_relevante ?r ?ing)
   (not (evidencia-incertidumbre (receta ?r) (criterio ?ing)))
   =>
   (assert (evidencia-incertidumbre
      (receta ?r) (criterio ?ing)
      (descripcion (str-cat "Te permite aprovechar un ingrediente que tienes: " ?ing))
      (valor (encadenado 1 (/ 3 10))))))

; -----------------------------------------------------------------------------
; Motor de Combinación de Certezas
; -----------------------------------------------------------------------------

(defrule INCERTIDUMBRE::combinar-evidencia-en-factor
   (declare (salience 0))
   ?fc <- (factor-certeza-receta (receta ?r) (fc ?actual))
   ?ev <- (evidencia-incertidumbre (receta ?r) (valor ?v) (aplicada no))
   =>
   (modify ?fc (fc (combinacion ?actual ?v)))
   (modify ?ev (aplicada si)))

; -----------------------------------------------------------------------------
; Cierre y cambio de módulo
; -----------------------------------------------------------------------------

(defrule INCERTIDUMBRE::pasar-a-presenta
   (declare (salience -100))
   ?m <- (modulo (nombre incertidumbre))
   (not (evidencia-incertidumbre (aplicada no)))
   =>
   (retract ?m)
   (assert (modulo (nombre presenta)))
   (focus PRESENTA))