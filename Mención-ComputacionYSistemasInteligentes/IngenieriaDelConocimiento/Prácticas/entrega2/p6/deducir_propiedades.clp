; AUTORES: JAIME CORZO GALDO, MARIO LINDEZ MARTINEZ

(defmodule DEDUCCION
   (import MAIN ?ALL)
   (export ?ALL)
)



(deffunction DEDUCCION::reemplaza-todo (?txt ?buscar ?reemplazo)
   (if (eq ?buscar "") then (return ?txt))
   (bind ?resultado "")
   (bind ?pos (str-index ?buscar ?txt))
   (while ?pos do
      (bind ?resultado
            (str-cat ?resultado
                     (sub-string 1 (- ?pos 1) ?txt)
                     ?reemplazo))
      (bind ?txt
            (sub-string (+ ?pos (str-length ?buscar))
                        (str-length ?txt)
                        ?txt))
      (bind ?pos (str-index ?buscar ?txt)))
   (return (str-cat ?resultado ?txt))
)

(deffunction DEDUCCION::normaliza-texto (?txt)
   (bind ?t (lowcase ?txt))

   ; Normalizacion de acentos y caracteres frecuentes
   (bind ?t (reemplaza-todo ?t "á" "a"))
   (bind ?t (reemplaza-todo ?t "à" "a"))
   (bind ?t (reemplaza-todo ?t "ä" "a"))
   (bind ?t (reemplaza-todo ?t "é" "e"))
   (bind ?t (reemplaza-todo ?t "è" "e"))
   (bind ?t (reemplaza-todo ?t "ë" "e"))
   (bind ?t (reemplaza-todo ?t "í" "i"))
   (bind ?t (reemplaza-todo ?t "ì" "i"))
   (bind ?t (reemplaza-todo ?t "ï" "i"))
   (bind ?t (reemplaza-todo ?t "ó" "o"))
   (bind ?t (reemplaza-todo ?t "ò" "o"))
   (bind ?t (reemplaza-todo ?t "ö" "o"))
   (bind ?t (reemplaza-todo ?t "ú" "u"))
   (bind ?t (reemplaza-todo ?t "ù" "u"))
   (bind ?t (reemplaza-todo ?t "ü" "u"))
   (bind ?t (reemplaza-todo ?t "ñ" "n"))

   ; Puntuacion y separadores
   (bind ?t (reemplaza-todo ?t "," " "))
   (bind ?t (reemplaza-todo ?t ";" " "))
   (bind ?t (reemplaza-todo ?t ":" " "))
   (bind ?t (reemplaza-todo ?t "." " "))
   (bind ?t (reemplaza-todo ?t "(" " "))
   (bind ?t (reemplaza-todo ?t ")" " "))
   (bind ?t (reemplaza-todo ?t "/" " "))
   (bind ?t (reemplaza-todo ?t "-" " "))
   (bind ?t (reemplaza-todo ?t "'" " "))
   (bind ?t (reemplaza-todo ?t "\"" " "))

   ; Espacios repetidos
   (while (str-index "  " ?t) do
      (bind ?t (reemplaza-todo ?t "  " " ")))

   ; Espacios a _
   (bind ?t (reemplaza-todo ?t " " "_"))
   (while (str-index "__" ?t) do
      (bind ?t (reemplaza-todo ?t "__" "_")))

   (return ?t)
)

(deffunction DEDUCCION::contiene-termino (?texto ?termino)
   (bind ?texto-normalizado (str-cat "_" (normaliza-texto ?texto) "_"))
   (bind ?termino-normalizado (normaliza-texto ?termino))
   (if (or (str-index (str-cat "_" ?termino-normalizado "_") ?texto-normalizado)
           (str-index (str-cat "_" ?termino-normalizado "s_") ?texto-normalizado)
           (str-index (str-cat "_" ?termino-normalizado "es_") ?texto-normalizado))
      then (return TRUE)
      else (return FALSE))
)

(deffunction DEDUCCION::kcal-de-texto (?txt)
   (if (numberp ?txt) then
      (return ?txt))

   (bind ?campos (explode$ (str-cat ?txt)))

   (if (> (length$ ?campos) 0) then
      (bind ?valor (nth$ 1 ?campos))
      (if (numberp ?valor) then
         (return ?valor))
   )

   (return FALSE)
)

(deffunction DEDUCCION::umbral-calorias-ligera (?tipo)
   (if (or (eq ?tipo principal) (eq ?tipo plato_principal)) then (return 400))
   (if (or (eq ?tipo postre) (eq ?tipo desayuno-merienda)) then (return 250))
   (if (or (eq ?tipo entrante) (eq ?tipo acompanamiento) (eq ?tipo salsa) (eq ?tipo bebida)) then (return 200))
   (return 300)
)

(deffunction DEDUCCION::umbral-calorias-normal (?tipo)
   (if (or (eq ?tipo principal) (eq ?tipo plato_principal)) then (return 650))
   (if (or (eq ?tipo postre) (eq ?tipo desayuno-merienda)) then (return 400))
   (if (or (eq ?tipo entrante) (eq ?tipo acompanamiento) (eq ?tipo salsa) (eq ?tipo bebida)) then (return 350))
   (return 500)
)

(defrule DEDUCCION::def_alimento
   (declare (salience 5))
   (es_un_tipo_de ?x ?y)
   =>
   (assert (es_alimento (sym-cat (lowcase (str-cat ?x)))))
)

(defrule DEDUCCION::identificar_alimento_mencionado_en_ingrediente
   (declare (salience 10))
   (ingrediente (nombre-receta ?r) (nombre-ingrediente ?texto))
   (or (es_alimento ?a) (es_grupo_alimentos ?a))
   (test (contiene-termino ?texto ?a))
   (not (ingrediente_corresponde ?r ?texto ?a))
=>
   (assert (ingrediente_corresponde ?r ?texto ?a))
)

(defrule DEDUCCION::deducir_ingrediente_relevante
   (declare (salience -10))
   (ingrediente_corresponde ?r ?texto ?a)
   (not (es_un_tipo_de ?a condimento))
   (not (and (ingrediente_corresponde ?r ?otro-texto ?b)
             (es_un_tipo_de ?b ?a)
             (test (neq ?a ?b))))
   (not (propiedad_receta ingrediente_relevante ?r ?a))
=>
   (assert (propiedad_receta ingrediente_relevante ?r ?a))
)

(defrule DEDUCCION::eliminar_ingrediente_relevante_generico
   ?f <- (propiedad_receta ingrediente_relevante ?r ?a)
   (propiedad_receta ingrediente_relevante ?r ?b)
   (es_un_tipo_de ?b ?a)
   (test (neq ?a ?b))
=>
   (retract ?f)
)

(defrule DEDUCCION::deducir_tipo_postre_por_nombre
   (declare (salience 60))
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (palabra_clave postre $? ?palabra $?)
   (test (contiene-termino ?nombre ?palabra))
=>
   (modify ?f (tipo-plato postre))
)

(defrule DEDUCCION::deducir_tipo_bebida_por_nombre
   (declare (salience 55))
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (palabra_clave bebida $? ?palabra $?)
   (test (contiene-termino ?nombre ?palabra))
=>
   (modify ?f (tipo-plato bebida))
)

(defrule DEDUCCION::deducir_tipo_salsa_por_nombre
   (declare (salience 50))
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (palabra_clave salsa $? ?palabra $?)
   (test (contiene-termino ?nombre ?palabra))
=>
   (modify ?f (tipo-plato salsa))
)

(defrule DEDUCCION::deducir_tipo_merienda_por_nombre
   (declare (salience 45))
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (palabra_clave desayuno-merienda $? ?palabra $?)
   (test (contiene-termino ?nombre ?palabra))
=>
   (modify ?f (tipo-plato desayuno-merienda))
)

(defrule DEDUCCION::deducir_tipo_entrante_por_nombre
   (declare (salience 40))
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (palabra_clave entrante $? ?palabra $?)
   (test (contiene-termino ?nombre ?palabra))
=>
   (modify ?f (tipo-plato entrante))
)

(defrule DEDUCCION::deducir_tipo_acompanamiento_por_nombre
   (declare (salience 35))
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (palabra_clave acompanamiento $? ?palabra $?)
   (test (contiene-termino ?nombre ?palabra))
=>
   (modify ?f (tipo-plato acompanamiento))
)

(defrule DEDUCCION::deducir_tipo_principal_por_nombre
   (declare (salience 30))
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (palabra_clave principal $? ?palabra $?)
   (test (contiene-termino ?nombre ?palabra))
=>
   (modify ?f (tipo-plato principal))
)

(defrule DEDUCCION::deducir_tipo_postre_por_ingredientes
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (ingrediente_corresponde ?nombre ? ?a1)
   (or (test (eq ?a1 azucar))
       (es_un_tipo_de ?a1 azucar)
       (test (eq ?a1 dulces))
       (es_un_tipo_de ?a1 dulces))
   (ingrediente_corresponde ?nombre ? ?a2)
   (or (test (eq ?a2 harina))
       (es_un_tipo_de ?a2 harina)
       (test (eq ?a2 lacteos))
       (es_un_tipo_de ?a2 lacteos)
       (test (eq ?a2 fruta))
       (es_un_tipo_de ?a2 fruta)
       (test (eq ?a2 chocolate))
       (test (eq ?a2 cacao))
       (test (eq ?a2 mantequilla)))
=>
   (modify ?f (tipo-plato postre))
)

(defrule DEDUCCION::deducir_tipo_principal_por_ingredientes
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (ingrediente_corresponde ?nombre ? ?a)
   (or (test (eq ?a carne))
       (es_un_tipo_de ?a carne)
       (test (eq ?a pescado))
       (es_un_tipo_de ?a pescado)
       (test (eq ?a pasta))
       (es_un_tipo_de ?a pasta)
       (test (eq ?a arroz))
       (test (eq ?a legumbres))
       (es_un_tipo_de ?a legumbres)
       (test (eq ?a huevos))
       (es_un_tipo_de ?a huevos)
       (test (eq ?a marisco))
       (es_un_tipo_de ?a marisco))
=>
   (modify ?f (tipo-plato principal))
)

(defrule DEDUCCION::deducir_tipo_acompanamiento_o_entrante_por_ingredientes
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (ingrediente_corresponde ?nombre ? ?a1)
   (or (test (eq ?a1 patata)) (test (eq ?a1 arroz)) (es_un_tipo_de ?a1 verdura) (es_un_tipo_de ?a1 hortalizas) (test (eq ?a1 tomate)))

   (not (and (ingrediente_corresponde ?nombre ? ?a2)
             (or (es_un_tipo_de ?a2 carne) (es_un_tipo_de ?a2 pescado) (es_un_tipo_de ?a2 pasta) (es_un_tipo_de ?a2 legumbres) (es_un_tipo_de ?a2 marisco))))
   (not (and (ingrediente_corresponde ?nombre ? ?a3)
             (or (test (eq ?a3 azucar)) (es_un_tipo_de ?a3 dulces))))
=>
   (if (or (eq ?a1 patata) (eq ?a1 arroz))
      then (modify ?f (tipo-plato acompanamiento))
      else (modify ?f (tipo-plato entrante)))
)

(defrule DEDUCCION::marcar_no_vegetariana
   (declare (salience 4))
   (ingrediente_corresponde ?r ? ?a)
   (or (es_un_tipo_de ?a carne)
       (es_un_tipo_de ?a pescado)
       (es_un_tipo_de ?a marisco)
       (es_un_tipo_de ?a embutidos)
       (es_un_tipo_de ?a fiambres))
   (not (propiedad_receta no_vegetariana ?r si))
=>
   (assert (propiedad_receta no_vegetariana ?r si))
)

(defrule DEDUCCION::marcar_no_vegana_por_origen_animal
   (declare (salience 4))
   (or (propiedad_receta no_vegetariana ?r si)
       (and (ingrediente_corresponde ?r ? ?a)
            (or (es_un_tipo_de ?a huevos)
                (es_un_tipo_de ?a lacteos)
                (test (eq ?a miel)))))
   (not (propiedad_receta no_vegana ?r si))
=>
   (assert (propiedad_receta no_vegana ?r si))
)

(defrule DEDUCCION::marcar_con_gluten_por_texto
   (declare (salience 4))
   (ingrediente (nombre-receta ?r) (nombre-ingrediente ?txt))
   (termino_gluten $? ?t $?)
   (test (contiene-termino ?txt ?t))
   (not (propiedad_receta con_gluten ?r si))
=>
   (assert (propiedad_receta con_gluten ?r si))
)

(defrule DEDUCCION::marcar_con_lactosa_por_texto
   (declare (salience 4))
   (ingrediente (nombre-receta ?r) (nombre-ingrediente ?txt))
   (termino_lactosa $? ?t $?)
   (test (contiene-termino ?txt ?t))
   (not (propiedad_receta con_lactosa ?r si))
=>
   (assert (propiedad_receta con_lactosa ?r si))
)

(defrule DEDUCCION::marcar_picante_por_ingrediente
   (declare (salience 4))
   (ingrediente (nombre-receta ?r) (nombre-ingrediente ?txt))
   (termino_picante $? ?t $?)
   (test (contiene-termino ?txt ?t))
   (not (propiedad_receta es_picante ?r si))
=>
   (assert (propiedad_receta es_picante ?r si))
)

(defrule DEDUCCION::marcar_picante_por_nombre
   (declare (salience 4))
   (receta (nombre ?r))
   (termino_picante $? ?t $?)
   (test (contiene-termino ?r ?t))
   (not (propiedad_receta es_picante ?r si))
=>
   (assert (propiedad_receta es_picante ?r si))
)

(defrule DEDUCCION::marcar_digestiva_pesada_por_ingredientes
   (declare (salience 4))
   (ingrediente_corresponde ?r ? ?a)
   (or (es_un_tipo_de ?a carne_roja)
       (es_un_tipo_de ?a embutidos)
       (test (eq ?a mantequilla))
       (test (eq ?a nata))
       (test (eq ?a mascarpone))
       (test (eq ?a ricotta)))
   (not (propiedad_receta digestion_pesada ?r si))
=>
   (assert (propiedad_receta digestion_pesada ?r si))
)

(defrule DEDUCCION::clasificar_vegetariana_no
   (declare (salience 3))
   (propiedad_receta no_vegetariana ?r si)
   (not (propiedad_receta es_vegetariana ?r no))
=>
   (assert (propiedad_receta es_vegetariana ?r no))
)

(defrule DEDUCCION::clasificar_vegetariana_si
   (declare (salience 3))
   (receta (nombre ?r))
   (not (propiedad_receta no_vegetariana ?r si))
   (not (propiedad_receta es_vegetariana ?r si))
=>
   (assert (propiedad_receta es_vegetariana ?r si))
)

(defrule DEDUCCION::clasificar_vegana_no
   (declare (salience 3))
   (propiedad_receta no_vegana ?r si)
   (not (propiedad_receta es_vegana ?r no))
=>
   (assert (propiedad_receta es_vegana ?r no))
)

(defrule DEDUCCION::clasificar_vegana_si
   (declare (salience 3))
   (receta (nombre ?r))
   (not (propiedad_receta no_vegana ?r si))
   (not (propiedad_receta es_vegana ?r si))
=>
   (assert (propiedad_receta es_vegana ?r si))
)

(defrule DEDUCCION::clasificar_sin_gluten_no
   (declare (salience 3))
   (propiedad_receta con_gluten ?r si)
   (not (propiedad_receta es_sin_gluten ?r no))
=>
   (assert (propiedad_receta es_sin_gluten ?r no))
)

(defrule DEDUCCION::clasificar_sin_gluten_si
   (declare (salience 3))
   (receta (nombre ?r))
   (not (propiedad_receta con_gluten ?r si))
   (not (propiedad_receta es_sin_gluten ?r si))
=>
   (assert (propiedad_receta es_sin_gluten ?r si))
)

(defrule DEDUCCION::clasificar_sin_lactosa_no
   (declare (salience 3))
   (propiedad_receta con_lactosa ?r si)
   (not (propiedad_receta es_sin_lactosa ?r no))
=>
   (assert (propiedad_receta es_sin_lactosa ?r no))
)

(defrule DEDUCCION::clasificar_sin_lactosa_si
   (declare (salience 3))
   (receta (nombre ?r))
   (not (propiedad_receta con_lactosa ?r si))
   (not (propiedad_receta es_sin_lactosa ?r si))
=>
   (assert (propiedad_receta es_sin_lactosa ?r si))
)

(defrule DEDUCCION::clasificar_picante_no
   (declare (salience 3))
   (receta (nombre ?r))
   (not (propiedad_receta es_picante ?r si))
   (not (propiedad_receta es_picante ?r no))
=>
   (assert (propiedad_receta es_picante ?r no))
)

(defrule DEDUCCION::clasificar_calorias_ligera
   (declare (salience 2))
   (receta (nombre ?r) (tipo-plato ?tipo) (info-nutricional ?kcal-texto $?))
   (test (neq (kcal-de-texto ?kcal-texto) FALSE))
   (test (< (kcal-de-texto ?kcal-texto) (umbral-calorias-ligera ?tipo)))
   (not (propiedad_receta calorias ?r ligera))
=>
   (assert (propiedad_receta calorias ?r ligera))
)

(defrule DEDUCCION::clasificar_calorias_normal
   (declare (salience 2))
   (receta (nombre ?r) (tipo-plato ?tipo) (info-nutricional ?kcal-texto $?))
   (test (neq (kcal-de-texto ?kcal-texto) FALSE))
   (test (>= (kcal-de-texto ?kcal-texto) (umbral-calorias-ligera ?tipo)))
   (test (<= (kcal-de-texto ?kcal-texto) (umbral-calorias-normal ?tipo)))
   (not (propiedad_receta calorias ?r normal))
=>
   (assert (propiedad_receta calorias ?r normal))
)

(defrule DEDUCCION::clasificar_calorias_calorica
   (declare (salience 2))
   (receta (nombre ?r) (tipo-plato ?tipo) (info-nutricional ?kcal-texto $?))
   (test (neq (kcal-de-texto ?kcal-texto) FALSE))
   (test (> (kcal-de-texto ?kcal-texto) (umbral-calorias-normal ?tipo)))
   (not (propiedad_receta calorias ?r calorica))
=>
   (assert (propiedad_receta calorias ?r calorica))
)

(defrule DEDUCCION::pasar-a-propuesta-tipo-receta
   (declare (salience -1000))
   ?m <- (modulo (nombre deducir-propiedades))
   =>
   (retract ?m)
   (assert (modulo (nombre propuesta-tipo-receta)))
   (focus PROPUESTA-TIPO-RECETA)
)


; ===============================================
;           GRUPOS CULINARIOS





(defrule DEDUCCION::familia_reposteria_por_tipo
   (declare (salience 2))
   (receta (nombre ?r) (tipo-plato ?tipo))
   (or (test (eq ?tipo postre))
       (test (eq ?tipo desayuno-merienda)))
   (not (propiedad_receta familia_plato ?r reposteria))
=>
   (assert (propiedad_receta familia_plato ?r reposteria))
)

(defrule DEDUCCION::perfil_ligero_por_propiedades
   (declare (salience 2))
   (receta (nombre ?r))
   (or (propiedad_receta calorias ?r ligera)
       (propiedad_receta digestion ?r ligera))
   (not (propiedad_receta perfil_plato ?r ligero))
=>
   (assert (propiedad_receta perfil_plato ?r ligero))
)

(defrule DEDUCCION::grupo_italiana_por_nombre
   (declare (salience 3))
   (receta (nombre ?r))
   (palabra_clave_grupo italiana $? ?p $?)
   (test (contiene-termino ?r ?p))
   (not (propiedad_receta grupo_culinario ?r italiana))
=>
   (assert (propiedad_receta grupo_culinario ?r italiana))
)

(defrule DEDUCCION::grupo_italiana_por_ingredientes
   (declare (salience 3))
   (receta (nombre ?r))
   (or
      (and
         (ingrediente_corresponde ?r ? ?a)
         (or (test (eq ?a pasta))
             (es_un_tipo_de ?a pasta)))
      (and
         (ingrediente_corresponde ?r ? ?a1)
         (test (eq ?a1 tomate))
         (ingrediente_corresponde ?r ? ?a2)
         (or (test (eq ?a2 queso))
             (es_un_tipo_de ?a2 queso)))
      (and
         (ingrediente (nombre-receta ?r) (nombre-ingrediente ?txt))
         (test (or (contiene-termino ?txt "pesto")
                   (contiene-termino ?txt "albahaca")
                   (contiene-termino ?txt "parmesano")
                   (contiene-termino ?txt "mozzarella")))))
   (not (propiedad_receta grupo_culinario ?r italiana))
=>
   (assert (propiedad_receta grupo_culinario ?r italiana))
)

(defrule DEDUCCION::grupo_oriental_por_nombre
   (declare (salience 3))
   (receta (nombre ?r))
   (palabra_clave_grupo oriental $? ?p $?)
   (test (contiene-termino ?r ?p))
   (not (propiedad_receta grupo_culinario ?r oriental))
=>
   (assert (propiedad_receta grupo_culinario ?r oriental))
)

(defrule DEDUCCION::grupo_oriental_por_ingredientes
   (declare (salience 3))
   (receta (nombre ?r))
   (ingrediente (nombre-receta ?r) (nombre-ingrediente ?txt))
   (test (or (contiene-termino ?txt "soja")
             (contiene-termino ?txt "teriyaki")
             (contiene-termino ?txt "jengibre")
             (contiene-termino ?txt "curry")
             (contiene-termino ?txt "fideos")))
   (not (propiedad_receta grupo_culinario ?r oriental))
=>
   (assert (propiedad_receta grupo_culinario ?r oriental))
)

(defrule DEDUCCION::grupo_mexicana_por_nombre
   (declare (salience 3))
   (receta (nombre ?r))
   (palabra_clave_grupo mexicana $? ?p $?)
   (test (contiene-termino ?r ?p))
   (not (propiedad_receta grupo_culinario ?r mexicana))
=>
   (assert (propiedad_receta grupo_culinario ?r mexicana))
)

(defrule DEDUCCION::grupo_mexicana_por_ingredientes
   (declare (salience 3))
   (receta (nombre ?r))
   (or
      (and
         (ingrediente (nombre-receta ?r) (nombre-ingrediente ?txt))
         (test (or (contiene-termino ?txt "chile")
                   (contiene-termino ?txt "jalapeno")
                   (contiene-termino ?txt "habanero")
                   (contiene-termino ?txt "chipotle")
                   (contiene-termino ?txt "aguacate")
                   (contiene-termino ?txt "guacamole")
                   (contiene-termino ?txt "frijol")
                   (contiene-termino ?txt "frejol")
                   (contiene-termino ?txt "tortilla")
                   (contiene-termino ?txt "maiz"))))
      (and
         (ingrediente (nombre-receta ?r) (nombre-ingrediente ?txt1))
         (test (or (contiene-termino ?txt1 "tortilla")
                   (contiene-termino ?txt1 "maiz")))
         (ingrediente (nombre-receta ?r) (nombre-ingrediente ?txt2))
         (test (or (contiene-termino ?txt2 "frijol")
                   (contiene-termino ?txt2 "frejol")
                   (contiene-termino ?txt2 "aguacate")))))
   (not (propiedad_receta grupo_culinario ?r mexicana))
=>
   (assert (propiedad_receta grupo_culinario ?r mexicana))
)

(defrule DEDUCCION::grupo_casera_por_nombre
   (declare (salience 2))
   (receta (nombre ?r))
   (palabra_clave_grupo casera $? ?p $?)
   (test (contiene-termino ?r ?p))
   (not (propiedad_receta grupo_culinario ?r casera))
=>
   (assert (propiedad_receta grupo_culinario ?r casera))
)

(defrule DEDUCCION::grupo_casera_por_ingredientes
   (declare (salience 2))
   (receta (nombre ?r) (tipo-plato principal))
   (or
      (and
         (ingrediente_corresponde ?r ? ?a)
         (or (test (eq ?a legumbres))
             (es_un_tipo_de ?a legumbres)))
      (and
         (ingrediente_corresponde ?r ? ?b)
         (test (eq ?b patata)))
      (and
         (ingrediente_corresponde ?r ? ?c)
         (test (eq ?c arroz))))
   (not (propiedad_receta grupo_culinario ?r italiana))
   (not (propiedad_receta grupo_culinario ?r oriental))
   (not (propiedad_receta grupo_culinario ?r mexicana))
   (not (propiedad_receta grupo_culinario ?r casera))
=>
   (assert (propiedad_receta grupo_culinario ?r casera))
)

(defrule DEDUCCION::grupo_mediterranea_por_ingredientes
   (declare (salience 1))
   (receta (nombre ?r))
   (or
      (and
         (ingrediente_corresponde ?r ? ?a)
         (or
            (test (eq ?a tomate))
            (test (eq ?a ajo))
            (test (eq ?a cebolla))
            (test (eq ?a pescado))
            (es_un_tipo_de ?a pescado)
            (test (eq ?a arroz))
            (test (eq ?a verdura))
            (es_un_tipo_de ?a verdura)
            (test (eq ?a legumbres))
            (es_un_tipo_de ?a legumbres)))
      (and
         (ingrediente (nombre-receta ?r) (nombre-ingrediente ?txt))
         (test (contiene-termino ?txt "aceite de oliva"))))
   (not (propiedad_receta grupo_culinario ?r italiana))
   (not (propiedad_receta grupo_culinario ?r oriental))
   (not (propiedad_receta grupo_culinario ?r mexicana))
   (not (propiedad_receta grupo_culinario ?r reposteria))
   (not (propiedad_receta grupo_culinario ?r mediterranea))
=>
   (assert (propiedad_receta grupo_culinario ?r mediterranea))
)

(defrule DEDUCCION::grupo_por_defecto_principal
   (declare (salience -1))
   (receta (nombre ?r) (tipo-plato principal))
   (not (propiedad_receta grupo_culinario ?r ?))
=>
   (assert (propiedad_receta grupo_culinario ?r casera))
)

(defrule DEDUCCION::grupo_por_defecto_no_principal
   (declare (salience -2))
   (receta (nombre ?r))
   (not (propiedad_receta grupo_culinario ?r ?))
=>
   (assert (propiedad_receta grupo_culinario ?r mediterranea))
)
