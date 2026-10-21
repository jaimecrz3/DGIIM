; AUTORES: JAIME CORZO GALDO, MARIO LINDEZ MARTINEZ

(defmodule PROPUESTA-TIPO-RECETA
   (import MAIN deftemplate preferencia)
   (import MAIN deftemplate modulo)
   (import MAIN deftemplate ingrediente-disponible)
   (import MAIN deftemplate estado-menu)
   (export ?ALL)
)

(deftemplate propuesta-inicial
   (slot grupo)
   (multislot motivos))

(deftemplate propuesta-mostrada
   (slot valor))

(deftemplate respuesta-propuesta
   (slot valor))


(defrule PROPUESTA-TIPO-RECETA::inicializar-propuesta
   (declare (salience 100))
   (modulo (nombre propuesta-tipo-receta))
   (not (propuesta-mostrada (valor si)))
   =>
   (do-for-all-facts
      ((?r respuesta-propuesta))
      TRUE
      (retract ?r))
   (do-for-all-facts
      ((?p propuesta-mostrada))
      TRUE
      (retract ?p))
)

; (defrule PROPUESTA-TIPO-RECETA::proponer_reposteria
;    (declare (salience 20))
;    (modulo (nombre propuesta-tipo-receta))
;    (preferencia (nombre tipo-plato) (valor postre))
;    (not (propuesta-inicial (grupo ?)))
; =>
;    (assert (propuesta-inicial
;       (grupo reposteria)
;       (motivos tipo-plato-postre)))
; )

; (defrule PROPUESTA-TIPO-RECETA::proponer_reposteria_merienda
;    (declare (salience 19))
;    (modulo (nombre propuesta-tipo-receta))
;    (preferencia (nombre tipo-plato) (valor merienda))
;    (not (propuesta-inicial (grupo ?)))
; =>
;    (assert (propuesta-inicial
;       (grupo reposteria)
;       (motivos tipo-plato-merienda)))
; )

; (defrule PROPUESTA-TIPO-RECETA::proponer_ligera
;    (declare (salience 18))
;    (modulo (nombre propuesta-tipo-receta))
;    (preferencia (nombre calorias) (valor ligera))
;    (not (propuesta-inicial (grupo ?)))
; =>
;    (assert (propuesta-inicial
;       (grupo ligera)
;       (motivos pocas-calorias)))
; )

(defrule PROPUESTA-TIPO-RECETA::proponer_oriental_por_picante_vegana
   (declare (salience 17))
   (modulo (nombre propuesta-tipo-receta))
   (preferencia (nombre picante) (valor si))
   (preferencia (nombre vegana) (valor si))
   (not (propuesta-inicial (grupo ?)))
=>
   (assert (propuesta-inicial
      (grupo oriental)
      (motivos picante vegana)))
)

(defrule PROPUESTA-TIPO-RECETA::proponer_mexicana_por_picante
   (declare (salience 16))
   (modulo (nombre propuesta-tipo-receta))
   (preferencia (nombre picante) (valor si))
   (not (preferencia (nombre vegana) (valor si)))
   (not (propuesta-inicial (grupo ?)))
=>
   (assert (propuesta-inicial
      (grupo mexicana)
      (motivos picante)))
)

(defrule PROPUESTA-TIPO-RECETA::proponer_italiana_por_ingredientes
   (declare (salience 15))
   (modulo (nombre propuesta-tipo-receta))
   (ingrediente-disponible (nombre tomate))
   (or (ingrediente-disponible (nombre pasta))
       (ingrediente-disponible (nombre queso)))
   (not (preferencia (nombre sin-gluten) (valor si)))
   (not (propuesta-inicial (grupo ?)))
=>
   (assert (propuesta-inicial
      (grupo italiana)
      (motivos ingredientes-disponibles)))
)

(defrule PROPUESTA-TIPO-RECETA::proponer_oriental_por_ingredientes
   (declare (salience 14))
   (modulo (nombre propuesta-tipo-receta))
   (or (ingrediente-disponible (nombre soja))
       (ingrediente-disponible (nombre jengibre))
       (ingrediente-disponible (nombre arroz)))
   (not (propuesta-inicial (grupo ?)))
=>
   (assert (propuesta-inicial
      (grupo oriental)
      (motivos ingredientes-disponibles)))
)

(defrule PROPUESTA-TIPO-RECETA::proponer_mediterranea_principal
   (declare (salience 10))
   (modulo (nombre propuesta-tipo-receta))
   (preferencia (nombre tipo-plato) (valor principal))
   (preferencia (nombre picante) (valor no))
   (not (propuesta-inicial (grupo ?)))
=>
   (assert (propuesta-inicial
      (grupo mediterranea)
      (motivos principal equilibrada)))
)

(defrule PROPUESTA-TIPO-RECETA::proponer_casera_por_defecto_principal
   (declare (salience 5))
   (modulo (nombre propuesta-tipo-receta))
   (preferencia (nombre tipo-plato) (valor principal))
   (not (propuesta-inicial (grupo ?)))
=>
   (assert (propuesta-inicial
      (grupo casera)
      (motivos principal-general)))
)

(defrule PROPUESTA-TIPO-RECETA::propuesta_por_defecto
   (declare (salience -10))
   (modulo (nombre propuesta-tipo-receta))
   (not (propuesta-inicial (grupo ?)))
=>
   (assert (propuesta-inicial
      (grupo mediterranea)
      (motivos defecto)))
)


(deffunction PROPUESTA-TIPO-RECETA::explicar-grupo (?grupo $?motivos)
   (printout t crlf "Te propongo empezar buscando recetas del grupo " ?grupo "." crlf)

   (if (member$ tipo-plato-postre $?motivos) then
      (printout t "La razon es que has indicado que buscas un postre." crlf))

   (if (member$ tipo-plato-merienda $?motivos) then
      (printout t "La razon es que has indicado que buscas una merienda." crlf))

   (if (member$ pocas-calorias $?motivos) then
      (printout t "La razon es que has pedido una receta ligera." crlf))

   (if (member$ picante $?motivos) then
      (printout t "La razon es que has indicado que te apetece algo picante." crlf))

   (if (and (member$ picante $?motivos)
            (member$ vegana $?motivos)) then
      (printout t "Ademas, has pedido que sea vegana, por eso una orientacion oriental puede encajar bien." crlf))

   (if (member$ ingredientes-disponibles $?motivos) then
      (printout t "Tambien he tenido en cuenta los ingredientes de los que dispones." crlf))

   (if (member$ equilibrada $?motivos) then
      (printout t "Parece una buena opcion equilibrada para un plato principal." crlf))

   (if (member$ principal-general $?motivos) then
      (printout t "Es una buena propuesta inicial para un plato principal de estilo tradicional." crlf))

   (if (member$ defecto $?motivos) then
      (printout t "Es una propuesta inicial general basada en tus preferencias." crlf))
)


(defrule PROPUESTA-TIPO-RECETA::mostrar_propuesta
   (modulo (nombre propuesta-tipo-receta))
   (propuesta-inicial (grupo ?g) (motivos $?m))
   (not (propuesta-mostrada (valor si)))
=>
   (explicar-grupo ?g $?m)
   (assert (propuesta-mostrada (valor si)))
)

(defrule PROPUESTA-TIPO-RECETA::preguntar_confirmacion
   (modulo (nombre propuesta-tipo-receta))
   (propuesta-inicial (grupo ?g))
   (propuesta-mostrada (valor si))
   (not (respuesta-propuesta (valor ?)))
=>
   (printout t crlf "¿Te parece bien este grupo inicial? (si/mas-restricciones): ")
   (assert (respuesta-propuesta (valor (read))))
)

(defrule PROPUESTA-TIPO-RECETA::aceptar_propuesta
   ?r <- (respuesta-propuesta (valor si))
   ?m <- (modulo (nombre propuesta-tipo-receta))
   ?pm <- (propuesta-mostrada (valor si))
=>
   (retract ?r)
   (retract ?m)
   (retract ?pm)
   (assert (modulo (nombre filtrar)))
   (focus FILTRA)
)

(defrule PROPUESTA-TIPO-RECETA::pedir_mas_restricciones
   ?r <- (respuesta-propuesta (valor mas-restricciones))
   ?m <- (modulo (nombre propuesta-tipo-receta))
   ?p <- (propuesta-inicial (grupo ?g) (motivos $?mot))
   ?pm <- (propuesta-mostrada (valor si))
=>
   (retract ?r)
   (retract ?m)
   (retract ?p)
   (retract ?pm)
   (assert (modulo (nombre preguntar)))
   (assert (estado-menu (valor activo)))
   (focus MAIN)
)

(defrule PROPUESTA-TIPO-RECETA::respuesta_no_valida
   ?r <- (respuesta-propuesta (valor ?x&~si&~mas-restricciones))
=>
   (retract ?r)
   (printout t "Respuesta no valida. Escribe si o mas-restricciones." crlf)
)