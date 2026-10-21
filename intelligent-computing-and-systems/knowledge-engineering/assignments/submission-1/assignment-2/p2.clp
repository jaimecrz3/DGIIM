; AUTORES: JAIME CORZO GALDO, MARIO LINDEZ MARTINEZ


;;;; AÑADIR LA INFORMACION DE AL MENOS 5 RECETAS NUEVAS (con al menos dos platos principales, un postre y un entrante)
;;;;; recoger los datos de recetas y anadir reglas de razonamiento en este fichero.

(deftemplate receta
    (slot nombre (type STRING))
    (slot tipo-plato (type SYMBOL) (default desconocido))
    (slot dificultad (type SYMBOL))
    (slot comensales (type INTEGER))
    (slot tiempo-cocinado (type INTEGER))
    (multislot info-nutricional)
    (slot enlace-web (type STRING))
)

(deftemplate ingrediente
    (slot nombre-receta (type STRING)) ; Enlace con la receta
    (slot nombre-ingrediente (type STRING))
    (slot cantidad (type FLOAT))
    (slot unidad (type SYMBOL)) ; g, ml, unidades, pizca...
)

;;;; OJO: para cargar las recetas tienes que cargar tambien SE_clasificar_alimentos.clp
;;;; y el fichero con las recetas de prueba antes de hacer reset/run.

; (defrule con_pollo
;     (ingrediente (nombre-receta ?r) (nombre-ingrediente ?a))
;     (test (str-index "pollo" (lowcase ?a)))
;     (not (propiedad_receta con_pollo ?r))
; =>
;     (assert (propiedad_receta con_pollo ?r))
;     (printout t ?r " contiene pollo" crlf)
; )

;;; FORMATO DE LOS HECHOS: (siendo ?r el nombre de la receta)
;;;
;;;   (propiedad_receta ingrediente_relevante ?r ?a)
;;;   (propiedad_receta digestion ligera/normal/pesada ?r)
;;;   (propiedad_receta calorias ligera/normal/calorica ?r)
;;;   (propiedad_receta es_vegetariana ?r)
;;;   (propiedad_receta es_vegana ?r)
;;;   (propiedad_receta es_sin_gluten ?r)
;;;   (propiedad_receta es_picante ?r)
;;;   (propiedad_receta es_sin_lactosa ?r)


;;; -----------------------------------------------------------------
;;;  1) DEDUCCION DE INGREDIENTES RELEVANTES
;;; -----------------------------------------------------------------
;;; Heuristica utilizada:
;;;   - Un ingrediente es relevante si corresponde a un alimento
;;;     identificable en la base de conocimiento.
;;;   - Los condimentos y auxiliares de sazonado (sal, aceite, vino,
;;;     especias, hierbas aromaticas, agua, salsas...) no se consideran
;;;     ingredientes relevantes.
;;;   - Cuando un mismo ingrediente encaja con un termino generico y con
;;;     otro mas especifico, se conserva el mas especifico.

;;; Ampliacion minima del conocimiento alimentario para reconocer mejor
;;; ingredientes habituales en recetas. Si el profesor prueba con nuevas
;;; recetas, basta con seguir ampliando aqui la relacion es_un_tipo_de.
(deffacts ampliacion_conocimiento_recetas
   (es_un_tipo_de pasta cereales)
   (es_un_tipo_de marisco pescado)
   (es_un_tipo_de salsa condimento)

   ;; --- PESCADOS Y MARISCOS ---
   (es_un_tipo_de merluza pescado)
   (es_un_tipo_de bacalao pescado)
   (es_un_tipo_de bonito pescado)
   (es_un_tipo_de pulpo marisco)
   (es_un_tipo_de gamba marisco)
   (es_un_tipo_de camaron marisco)
   (es_un_tipo_de almeja marisco)
   (es_un_tipo_de chirla marisco)

   ;; --- CARNES Y EMBUTIDOS ---
   (es_un_tipo_de pato carne_blanca)
   (es_un_tipo_de solomillo carne_roja)
   (es_un_tipo_de carne_picada carne_roja)
   (es_un_tipo_de morcilla embutidos)
   (es_un_tipo_de panceta embutidos)
   (es_un_tipo_de tocino embutidos)
   (es_un_tipo_de jamon embutidos)
   (es_un_tipo_de guanciale embutidos)
   (es_un_tipo_de charqui carne_roja)

   ;; --- VERDURAS Y HORTALIZAS ---
   (es_un_tipo_de esparrago verdura)
   (es_un_tipo_de seta verdura)
   (es_un_tipo_de champinon verdura)
   (es_un_tipo_de patata verdura)
   (es_un_tipo_de papa verdura)
   (es_un_tipo_de puerro verdura)
   (es_un_tipo_de apio verdura)
   (es_un_tipo_de calabaza verdura)
   (es_un_tipo_de berenjena verdura)
   (es_un_tipo_de repollo verdura)
   (es_un_tipo_de pepino verdura)
   (es_un_tipo_de hinojo verdura)
   (es_un_tipo_de calabacin verdura)
   (es_un_tipo_de zapallito verdura)
   (es_un_tipo_de nora verdura)
   (es_un_tipo_de chile verdura)

   ;; --- FRUTAS ---
   (es_un_tipo_de limon fruta)
   (es_un_tipo_de mango fruta)
   (es_un_tipo_de aguacate fruta)
   (es_un_tipo_de coco fruta)
   (es_un_tipo_de fresa fruta)
   (es_un_tipo_de arandano fruta)
   (es_un_tipo_de mandarina fruta)
   (es_un_tipo_de aceituna fruta)

   ;; --- LÁCTEOS Y HUEVOS ---
   (es_un_tipo_de mantequilla lacteos)
   (es_un_tipo_de nata lacteos)
   (es_un_tipo_de mascarpone queso)
   (es_un_tipo_de ricotta queso)
   (es_un_tipo_de huevo huevos) 

   ;; --- CEREALES Y PASTA ---
   (es_un_tipo_de espagueti pasta)
   (es_un_tipo_de fideo pasta)
   (es_un_tipo_de macarron pasta)
   (es_un_tipo_de noqui pasta)
   (es_un_tipo_de arroz cereales)
   (es_un_tipo_de avena cereales)
   (es_un_tipo_de pan cereales)
   (es_un_tipo_de galleta cereales)
   (es_un_tipo_de hojaldre cereales)
   (es_un_tipo_de maicena harina)
   (es_un_tipo_de harina_de_trigo harina)

   ;; --- LEGUMBRES Y FRUTOS SECOS ---
   (es_un_tipo_de cacahuate frutos_secos)
   (es_un_tipo_de mani frutos_secos)
   (es_un_tipo_de pecana frutos_secos)
   (es_un_tipo_de pistacho frutos_secos)
   (es_un_tipo_de nuez frutos_secos)
   (es_un_tipo_de pinon frutos_secos)
   (es_un_tipo_de frejol legumbres)
   (es_un_tipo_de haba legumbres)
   (es_un_tipo_de lenteja legumbres)
   (es_un_tipo_de ajonjoli frutos_secos)
   (es_un_tipo_de sesamo frutos_secos)

   ;; --- HIERBAS, ESPECIAS Y CONDIMENTOS ---
   (es_un_tipo_de guindilla especies)
   (es_un_tipo_de vainilla especies)
   (es_un_tipo_de canela especies)
   (es_un_tipo_de oregano hierbas_aromaticas)
   (es_un_tipo_de comino especies)
   (es_un_tipo_de cilantro hierbas_aromaticas)
   (es_un_tipo_de eneldo hierbas_aromaticas)
   (es_un_tipo_de jengibre especies)
   (es_un_tipo_de albahaca hierbas_aromaticas)
   (es_un_tipo_de nuez_moscada especies)
   (es_un_tipo_de azafran especies)
   (es_un_tipo_de laurel especies)
   
   ;; --- SALSAS Y OTROS CONDIMENTOS ---
   (es_un_tipo_de soja salsa)
   (es_un_tipo_de teriyaki salsa)
   (es_un_tipo_de barbacoa salsa)
   (es_un_tipo_de mostaza salsa)
   (es_un_tipo_de vinagre condimento)
   (es_un_tipo_de levadura condimento)
   (es_un_tipo_de aceite_de_oliva aceite)

   ;; --- DULCES ---
   (es_un_tipo_de miel dulces)
   (es_un_tipo_de chocolate dulces)
   (es_un_tipo_de cacao dulces)
)

; Cada vez que encontramos un hecho (es_un_tipo_de merluza pescado) crea (es_alimento merluza)
; Con str-cat convertimos el simbolo en texto, lo ponemos en mayuscula y lo volvemos a convertir en simbolo
(defrule def_alimento
   (declare (salience 5))
   (es_un_tipo_de ?x ?y)
   =>
   ;; Guardamos el alimento siempre como un símbolo en minúsculas
   (assert (es_alimento (sym-cat (lowcase (str-cat ?x)))))
)

; FUNCIONES AUXILIARES
;
; Reemplaza todas las apariciones de un texto dentro de otro. 
; Ej: entrada: "lomos-de-merluza"
;     salida: "lomos de merluza"
(deffunction reemplaza-todo (?txt ?buscar ?reemplazo)
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

; Convierte un texto a una forma normalizada para comparar mejor:
; -> Pasa todo a minusculas
; -> Elimina o sustituye signos de puntuacion
; -> quita espacios dobles
; Ej: "Lomos de Merluza, frescos" acaba asi: lomos_de_merluza_frescos
; Con esto conseguimos poder comparar nombres de ingredientes sin importar el formato en el que vengan
(deffunction normaliza-texto (?txt)
   (bind ?t (lowcase ?txt))
   (bind ?t (reemplaza-todo ?t "," " "))
   (bind ?t (reemplaza-todo ?t ";" " "))
   (bind ?t (reemplaza-todo ?t ":" " "))
   (bind ?t (reemplaza-todo ?t "." " "))
   (bind ?t (reemplaza-todo ?t "(" " "))
   (bind ?t (reemplaza-todo ?t ")" " "))
   (bind ?t (reemplaza-todo ?t "/" " "))
   (bind ?t (reemplaza-todo ?t "-" " "))
   (bind ?t (reemplaza-todo ?t "'" " "))
   (bind ?t (reemplaza-todo ?t "  " " "))
   (while (str-index "  " ?t) do
      (bind ?t (reemplaza-todo ?t "  " " ")))
   (bind ?t (reemplaza-todo ?t " " "_"))
   (while (str-index "__" ?t) do
      (bind ?t (reemplaza-todo ?t "__" "_")))
   (return ?t)
)

; Comprueba si un término aparece dentro de un texto, incluyendo variantes de plural
; Ej: "lomos de merluza" contiene merluza
;     "gambas al ajillo" contiene gamba
(deffunction contiene-termino (?texto ?termino)
   (bind ?texto-normalizado (str-cat "_" (normaliza-texto ?texto) "_"))
   (bind ?termino-normalizado (normaliza-texto ?termino))
   (if (or (str-index (str-cat "_" ?termino-normalizado "_") ?texto-normalizado)
           (str-index (str-cat "_" ?termino-normalizado "s_") ?texto-normalizado)
           (str-index (str-cat "_" ?termino-normalizado "es_") ?texto-normalizado))
      then (return TRUE)
      else (return FALSE))
)

; Busca alimentos reconocibles dentro del nombre textual del ingrediente
; Ej: Si tienes (ingrediente (nombre-receta "Merluza al horno") (nombre-ingrediente "lomos de merluza"))
;     y existe (es_alimento merluza) entonces:
;     (ingrediente_corresponde "Merluza al horno" "lomos de merluza" merluza)
; No modifica el ingrediente original
; ingrediente_corresponde es un hecho auxiliar: 
; (ingrediente_corresponde receta texto-original alimento-detectado)
(defrule identificar_alimento_mencionado_en_ingrediente
   (declare (salience 10))
   (ingrediente (nombre-receta ?r) (nombre-ingrediente ?texto))
   (or (es_alimento ?a) (es_grupo_alimentos ?a))
   (test (contiene-termino ?texto ?a))
   (not (ingrediente_corresponde ?r ?texto ?a))
=>
   (assert (ingrediente_corresponde ?r ?texto ?a))
)

; Decide qué ingredientes son realmente relevantes para la receta
; Un ingrediente es relevante si:
; 1. ha sido reconocido antes con ingrediente_corresponde
; 2. no es un condimento
; 3. no existe otro ingrediente más específico (ej. si detecta merluza y pescado te quedas con merluza)
; 4. aún no se había marcado como relevante
;
; Crea un hecho como (propiedad_receta ingrediente_relevante "Merluza al horno" merluza)
; Donde propiedad_receta es un hecho auxiliar que tiene esta forma:
; (propiedad_receta propiedad nombre-receta valor)
; Ej: (propiedad_receta ingrediente_relevante "Paella" arroz)
;     (propiedad_receta digestion "Paella" pesada)
;     (propiedad_receta calorias "Tarta" calorica)
(defrule deducir_ingrediente_relevante
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

; Si por algún motivo quedaron marcados como relevantes dos ingredientes y 
; uno es genérico y otro específico, elimina el genérico
(defrule eliminar_ingrediente_relevante_generico
   ?f <- (propiedad_receta ingrediente_relevante ?r ?a)
   (propiedad_receta ingrediente_relevante ?r ?b)
   (es_un_tipo_de ?b ?a)
   (test (neq ?a ?b))
=>
   (retract ?f)
)

; DEDUCCION DEL TIPO DE PLATO CUANDO FALTA

(deffunction anunciar-tipo-plato-deducido (?nombre ?tipo)
   (printout t "Se ha deducido que '" ?nombre "' es de tipo " ?tipo "." crlf)
)

;;; Agrupamos las palabras clave que definentipos de plato por su nombre
;;; Esto es mucho mas limpio que ponerlas dentro de los 'test' de las reglas
(deffacts palabras_clave_tipo_plato
    (palabra_clave salsa "salsa" "alioli" "mayonesa" "vinagreta" "pesto" "bechamel" "romesco" "mojo")
    (palabra_clave bebida "zumo" "jugo" "batido" "smoothie" "limonada" "horchata" "granizado" "infusion" "cafe" "te")
    (palabra_clave postre "tarta" "bizcocho" "galleta" "flan" "helado" "mousse" "brownie" "coulant" "natillas" "cannoli" "torrija" "paparajote")
    (palabra_clave merienda "sandwich" "bocadillo" "tostada" "crepe" "tortitas" "croissant" "napolitana")
    (palabra_clave entrante "ensalada" "salmorejo" "gazpacho" "sopa" "crema" "hummus" "carpaccio" "bruschetta" "tosta" "buñuelos")
    (palabra_clave acompanamiento "guarnicion" "acompañamiento" "pure" "patatas fritas" "patatas asadas" "arroz blanco" "verduras salteadas")
    (palabra_clave principal "espagueti" "macarrones" "solomillo" "lomo" "entrecot" "pizza" "hamburguesa" "lasaña" "paella" "guiso" "potaje")
)

; A. DEDUCCION POR NOMBRE
; Si el nombre de la receta contiene una palabra clave, asigna el tipo de plato
; Ej: Si la receta se llama "Tarta de manzana" y existe (palabra_clave postre "tarta")
;     entonces indicamos que su tipo de plato es postre
; Solo actua si es tipo de plato es desconocido
(defrule deducir_tipo_por_nombre
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (palabra_clave ?tipo $? ?palabra $?) ; toma un hecho palabra clave, y la palabra puede aparecer en cualquier posicion
   (test (contiene-termino ?nombre ?palabra))
=>
   (modify ?f (tipo-plato ?tipo))
   (anunciar-tipo-plato-deducido ?nombre ?tipo)
)


;B. DEDUCCION POR INGREDIENTES (Si el nombre no ha funcionado)

;; B.1 Postre (Azucar + algo de postre)
(defrule deducir_tipo_postre_por_ingredientes
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (ingrediente_corresponde ?nombre ? ?a1)
   (or (test (eq ?a1 azucar)) (es_un_tipo_de ?a1 azucar) (es_un_tipo_de ?a1 dulces))
   (ingrediente_corresponde ?nombre ? ?a2)
   (or (test (eq ?a2 harina)) (es_un_tipo_de ?a2 harina) (es_un_tipo_de ?a2 lacteos) (es_un_tipo_de ?a2 fruta) 
       (test (eq ?a2 chocolate)) (test (eq ?a2 cacao)) (test (eq ?a2 mantequilla)))
=>
   (modify ?f (tipo-plato postre))
   (anunciar-tipo-plato-deducido ?nombre postre)
)

;; B.2 Principal (Carne, pescado, pasta, legumbres...)
(defrule deducir_tipo_principal_por_ingredientes
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (ingrediente_corresponde ?nombre ? ?a)
   (or (es_un_tipo_de ?a carne) (es_un_tipo_de ?a pescado) (es_un_tipo_de ?a pasta)
       (test (eq ?a arroz)) (es_un_tipo_de ?a legumbres) (es_un_tipo_de ?a huevos) (es_un_tipo_de ?a marisco))
=>
   (modify ?f (tipo-plato principal))
   (anunciar-tipo-plato-deducido ?nombre principal)
)

;; B.3 Acompañamiento o Entrante (Verduras/Patata SIN ingredientes fuertes ni azucar)
(defrule deducir_tipo_acompanamiento_o_entrante_por_ingredientes
   ?f <- (receta (nombre ?nombre) (tipo-plato desconocido))
   (ingrediente_corresponde ?nombre ? ?a1)
   (or (test (eq ?a1 patata)) (test (eq ?a1 arroz)) (es_un_tipo_de ?a1 verdura) (es_un_tipo_de ?a1 hortalizas) (test (eq ?a1 tomate)))
   
   ;; Verificamos que NO tenga carne, pescado, pasta, etc.
   (not (and (ingrediente_corresponde ?nombre ? ?a2)
             (or (es_un_tipo_de ?a2 carne) (es_un_tipo_de ?a2 pescado) (es_un_tipo_de ?a2 pasta) (es_un_tipo_de ?a2 legumbres) (es_un_tipo_de ?a2 marisco))))
   ;; Verificamos que NO tenga azucar
   (not (and (ingrediente_corresponde ?nombre ? ?a3)
             (or (test (eq ?a3 azucar)) (es_un_tipo_de ?a3 dulces))))
=>
   ;; Diferenciamos acompanamiento de entrante segun si tiene patata/arroz o solo verdura
   (if (or (eq ?a1 patata) (eq ?a1 arroz))
      then (modify ?f (tipo-plato acompanamiento))
           (anunciar-tipo-plato-deducido ?nombre acompanamiento)
      else (modify ?f (tipo-plato entrante))
           (anunciar-tipo-plato-deducido ?nombre entrante))
)

;;; =========================================================
;;; 3) CLASIFICACION DIETETICA DE LA RECETA
;;;    - vegana / vegetariana
;;;    - picante
;;;    - sin gluten / sin lactosa
;;;    - calorias ligera / normal / calorica
;;;    - digestion ligera / normal / pesada
;;; =========================================================

(deffacts terminos_clasificacion_recetas
   (termino_gluten "trigo" "pan" "galleta" "hojaldre" "espagueti" "fideo" "macarron" "pasta" "cuscus" "cerveza")
   (termino_lactosa "leche" "queso" "nata" "mantequilla" "yogur" "mascarpone" "ricotta" "bechamel")
   (termino_picante "guindilla" "chile" "cayena" "jalapeno" "tabasco" "wasabi" "harissa" "curry" "pimenton_picante")
)

(deffunction kcal-de-texto (?txt)
   ;; Si ya es un número (int o float), lo devolvemos directamente
   (if (numberp ?txt) then
      (return ?txt))
   
   ;; Lo convertimos a string y lo separamos (ej: "285 kcal" -> 285 y kcal)
   (bind ?campos (explode$ (str-cat ?txt)))
   
   ;; Comprobamos que no esté vacío
   (if (> (length$ ?campos) 0) then
      (bind ?valor (nth$ 1 ?campos))
      ;; Si el primer elemento extraído es un número, lo devolvemos
      (if (numberp ?valor) then
         (return ?valor)
      )
   )
   
   ;; Valor por defecto (normal) si la receta no tiene las kcal bien definidas 
   ;; (ej. "crema verduras ligero") para evitar que el programa aborte.
   (return 500) 
)

(deffunction umbral-calorias-ligera (?tipo)
   (if (or (eq ?tipo principal) (eq ?tipo plato_principal)) then (return 400))
   (if (or (eq ?tipo postre) (eq ?tipo merienda)) then (return 250))
   (if (or (eq ?tipo entrante) (eq ?tipo acompanamiento) (eq ?tipo salsa) (eq ?tipo bebida)) then (return 200))
   (return 300)
)

(deffunction umbral-calorias-normal (?tipo)
   (if (or (eq ?tipo principal) (eq ?tipo plato_principal)) then (return 650))
   (if (or (eq ?tipo postre) (eq ?tipo merienda)) then (return 400))
   (if (or (eq ?tipo entrante) (eq ?tipo acompanamiento) (eq ?tipo salsa) (eq ?tipo bebida)) then (return 350))
   (return 500)
)

(deffunction etiqueta-calorias (?valor)
   (if (eq ?valor calorica) then (return alta))
   (return ?valor)
)

(defrule marcar_no_vegetariana
   (declare (salience 4))
   (ingrediente_corresponde ?r ? ?a)
   (or (es_un_tipo_de ?a carne)
       (es_un_tipo_de ?a pescado)
       (es_un_tipo_de ?a marisco)
       (es_un_tipo_de ?a embutidos)
       (es_un_tipo_de ?a fiambres))
   (not (rasgo_receta no_vegetariana ?r))
=>
   (assert (rasgo_receta no_vegetariana ?r))
)

(defrule marcar_no_vegana_por_origen_animal
   (declare (salience 4))
   (or (rasgo_receta no_vegetariana ?r)
       (and (ingrediente_corresponde ?r ? ?a)
            (or (es_un_tipo_de ?a huevos)
                (es_un_tipo_de ?a lacteos)
                (test (eq ?a miel)))))
   (not (rasgo_receta no_vegana ?r))
=>
   (assert (rasgo_receta no_vegana ?r))
)

(defrule marcar_con_gluten_por_texto
   (declare (salience 4))
   (ingrediente (nombre-receta ?r) (nombre-ingrediente ?txt))
   (termino_gluten $? ?t $?)
   (test (contiene-termino ?txt ?t))
   (not (rasgo_receta con_gluten ?r))
=>
   (assert (rasgo_receta con_gluten ?r))
)

(defrule marcar_con_lactosa_por_texto
   (declare (salience 4))
   (ingrediente (nombre-receta ?r) (nombre-ingrediente ?txt))
   (termino_lactosa $? ?t $?)
   (test (contiene-termino ?txt ?t))
   (not (rasgo_receta con_lactosa ?r))
=>
   (assert (rasgo_receta con_lactosa ?r))
)

(defrule marcar_picante_por_ingrediente
   (declare (salience 4))
   (ingrediente (nombre-receta ?r) (nombre-ingrediente ?txt))
   (termino_picante $? ?t $?)
   (test (contiene-termino ?txt ?t))
   (not (rasgo_receta es_picante ?r))
=>
   (assert (rasgo_receta es_picante ?r))
)

(defrule marcar_picante_por_nombre
   (declare (salience 4))
   (receta (nombre ?r))
   (termino_picante $? ?t $?)
   (test (contiene-termino ?r ?t))
   (not (rasgo_receta es_picante ?r))
=>
   (assert (rasgo_receta es_picante ?r))
)

(defrule marcar_digestiva_pesada_por_ingredientes
   (declare (salience 4))
   (ingrediente_corresponde ?r ? ?a)
   (or (es_un_tipo_de ?a carne_roja)
       (es_un_tipo_de ?a embutidos)
       (test (eq ?a mantequilla))
       (test (eq ?a nata))
       (test (eq ?a mascarpone))
       (test (eq ?a ricotta)))
   (not (rasgo_receta digestion_pesada ?r))
=>
   (assert (rasgo_receta digestion_pesada ?r))
)

(defrule clasificar_vegetariana_no
   (declare (salience 3))
   (rasgo_receta no_vegetariana ?r)
   (not (propiedad_receta es_vegetariana ?r no))
=>
   (assert (propiedad_receta es_vegetariana ?r no))
)

(defrule clasificar_vegetariana_si
   (declare (salience 3))
   (receta (nombre ?r))
   (not (rasgo_receta no_vegetariana ?r))
   (not (propiedad_receta es_vegetariana ?r si))
=>
   (assert (propiedad_receta es_vegetariana ?r si))
)

(defrule clasificar_vegana_no
   (declare (salience 3))
   (rasgo_receta no_vegana ?r)
   (not (propiedad_receta es_vegana ?r no))
=>
   (assert (propiedad_receta es_vegana ?r no))
)

(defrule clasificar_vegana_si
   (declare (salience 3))
   (receta (nombre ?r))
   (not (rasgo_receta no_vegana ?r))
   (not (propiedad_receta es_vegana ?r si))
=>
   (assert (propiedad_receta es_vegana ?r si))
)

(defrule clasificar_sin_gluten_no
   (declare (salience 3))
   (rasgo_receta con_gluten ?r)
   (not (propiedad_receta es_sin_gluten ?r no))
=>
   (assert (propiedad_receta es_sin_gluten ?r no))
)

(defrule clasificar_sin_gluten_si
   (declare (salience 3))
   (receta (nombre ?r))
   (not (rasgo_receta con_gluten ?r))
   (not (propiedad_receta es_sin_gluten ?r si))
=>
   (assert (propiedad_receta es_sin_gluten ?r si))
)

(defrule clasificar_sin_lactosa_no
   (declare (salience 3))
   (rasgo_receta con_lactosa ?r)
   (not (propiedad_receta es_sin_lactosa ?r no))
=>
   (assert (propiedad_receta es_sin_lactosa ?r no))
)

(defrule clasificar_sin_lactosa_si
   (declare (salience 3))
   (receta (nombre ?r))
   (not (rasgo_receta con_lactosa ?r))
   (not (propiedad_receta es_sin_lactosa ?r si))
=>
   (assert (propiedad_receta es_sin_lactosa ?r si))
)

(defrule clasificar_picante_si
   (declare (salience 3))
   (rasgo_receta es_picante ?r)
   (not (propiedad_receta es_picante ?r si))
=>
   (assert (propiedad_receta es_picante ?r si))
)

(defrule clasificar_picante_no
   (declare (salience 3))
   (receta (nombre ?r))
   (not (rasgo_receta es_picante ?r))
   (not (propiedad_receta es_picante ?r no))
=>
   (assert (propiedad_receta es_picante ?r no))
)

(defrule clasificar_calorias_ligera
   (declare (salience 2))
   (receta (nombre ?r) (tipo-plato ?tipo) (info-nutricional ?kcal-texto $?))
   (test (< (kcal-de-texto ?kcal-texto) (umbral-calorias-ligera ?tipo)))
   (not (propiedad_receta calorias ?r ligera))
=>
   (assert (propiedad_receta calorias ?r ligera))
)

(defrule clasificar_calorias_normal
   (declare (salience 2))
   (receta (nombre ?r) (tipo-plato ?tipo) (info-nutricional ?kcal-texto $?))
   (test (>= (kcal-de-texto ?kcal-texto) (umbral-calorias-ligera ?tipo)))
   (test (<= (kcal-de-texto ?kcal-texto) (umbral-calorias-normal ?tipo)))
   (not (propiedad_receta calorias ?r normal))
=>
   (assert (propiedad_receta calorias ?r normal))
)

(defrule clasificar_calorias_calorica
   (declare (salience 2))
   (receta (nombre ?r) (tipo-plato ?tipo) (info-nutricional ?kcal-texto $?))
   (test (> (kcal-de-texto ?kcal-texto) (umbral-calorias-normal ?tipo)))
   (not (propiedad_receta calorias ?r calorica))
=>
   (assert (propiedad_receta calorias ?r calorica))
)

(defrule clasificar_digestion_pesada
   (declare (salience 1))
   (receta (nombre ?r) (tipo-plato ?tipo))
   (or (rasgo_receta digestion_pesada ?r)
       (and (propiedad_receta calorias ?r calorica)
            (or (test (eq ?tipo principal))
                (test (eq ?tipo postre))
                (test (eq ?tipo merienda)))))
   (not (propiedad_receta digestion ?r pesada))
=>
   (assert (propiedad_receta digestion ?r pesada))
)

(defrule clasificar_digestion_ligera
   (declare (salience 1))
   (receta (nombre ?r) (tipo-plato ?tipo))
   (not (rasgo_receta digestion_pesada ?r))
   (propiedad_receta calorias ?r ligera)
   (or (test (eq ?tipo entrante))
       (test (eq ?tipo acompanamiento))
       (and (test (eq ?tipo principal))
            (or (not (rasgo_receta no_vegetariana ?r))
                (and (ingrediente_corresponde ?r ? ?a)
                     (es_un_tipo_de ?a pescado)))))
   (not (propiedad_receta digestion ?r ligera))
=>
   (assert (propiedad_receta digestion ?r ligera))
)

(defrule clasificar_digestion_normal
   (declare (salience 0))
   (receta (nombre ?r))
   (not (propiedad_receta digestion ?r pesada))
   (not (propiedad_receta digestion ?r ligera))
   (not (propiedad_receta digestion ?r normal))
=>
   (assert (propiedad_receta digestion ?r normal))
)

; MENU

;;; 1. Preguntar si se quieren listar las recetas
(defrule inicio_preguntar_listar
    (declare (salience -100))
    =>
    (printout t crlf "¿Quieres ver la lista de todas las recetas disponibles? (si/no): ")
    (bind ?respuesta (read))
    (if (eq ?respuesta si) then
        (assert (listar_recetas))
    )
    (assert (pedir_receta))
)

;;; 2. Lógica para listar recetas (solo si el usuario dijo 'si')
(defrule listar_recetas_titulo
    (declare (salience -101))
    (listar_recetas)
    =>
    (printout t crlf "--- RECETAS DISPONIBLES ---" crlf)
)

(defrule listar_recetas_item
    (declare (salience -102))
    (listar_recetas)
    (receta (nombre ?nombre))
    =>
    (printout t " - " ?nombre crlf)
)

(defrule fin_listar_recetas
    (declare (salience -103))
    ?f <- (listar_recetas)
    =>
    (retract ?f)
    (printout t "---------------------------" crlf)
)

;;; 3. Preguntar por la receta específica
; Genera un hecho: (buscar_ingredientes_de "Merluza a la donostiarra")
(defrule preguntar_por_receta
    (declare (salience -104))
    ?f <- (pedir_receta)
    =>
    (printout t crlf "Escribe el nombre exacto de la receta: ")
    (bind ?nombre_receta (readline))
    (assert (buscar_ingredientes_de ?nombre_receta))
    (retract ?f)
)

;;; 4. Imprimir los ingredientes relevantes (Paso 3)
(defrule receta_no_encontrada
    (declare (salience -105))
    ?f <- (buscar_ingredientes_de ?receta)
    (not (receta (nombre ?receta)))
    =>
    (printout t crlf "No existe ninguna receta con ese nombre." crlf)
    (retract ?f)
)

(defrule imprimir_cabecera_busqueda
    (declare (salience -106))
    (buscar_ingredientes_de ?receta)
    (receta (nombre ?receta))
    (not (cabecera_impresa ?receta))
    =>
    (printout t crlf "Ingredientes relevantes de '" ?receta "':" crlf)
    (assert (cabecera_impresa ?receta))
)

(defrule imprimir_resultado_busqueda
    (declare (salience -107))
    (buscar_ingredientes_de ?receta)
    (propiedad_receta ingrediente_relevante ?receta ?ingrediente)
    =>
    (printout t " -> Ingrediente relevante: " ?ingrediente crlf)
)

(defrule imprimir_resumen_clasificacion
   (declare (salience -108))
   (buscar_ingredientes_de ?receta)
   (propiedad_receta es_vegana ?receta ?vegana)
   (propiedad_receta es_vegetariana ?receta ?vegetariana)
   (propiedad_receta es_picante ?receta ?picante)
   (propiedad_receta es_sin_gluten ?receta ?sin_gluten)
   (propiedad_receta es_sin_lactosa ?receta ?sin_lactosa)
   (propiedad_receta calorias ?receta ?calorias)
   (propiedad_receta digestion ?receta ?digestion)
   (not (resumen_clasificacion_impreso ?receta))
=>
   (printout t crlf "Clasificacion de '" ?receta "':" crlf)
   (printout t " - Vegana: " ?vegana crlf)
   (printout t " - Vegetariana: " ?vegetariana crlf)
   (printout t " - Picante: " ?picante crlf)
   (printout t " - Sin gluten: " ?sin_gluten crlf)
   (printout t " - Sin lactosa: " ?sin_lactosa crlf)
   (printout t " - Calorias: " (etiqueta-calorias ?calorias) crlf)
   (printout t " - Digestion: " ?digestion crlf)
   (assert (resumen_clasificacion_impreso ?receta))
)