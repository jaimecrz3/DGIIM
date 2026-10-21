; AUTORES: JAIME CORZO GALDO, MARIO LINDEZ MARTINEZ

(defmodule MAIN (export ?ALL))

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
    (slot nombre-receta (type STRING))
    (slot nombre-ingrediente (type STRING))
    (slot cantidad (type FLOAT))
    (slot unidad (type SYMBOL))
)

(deffacts ampliacion_conocimiento_recetas
   (es_un_tipo_de pasta cereales)
   (es_un_tipo_de marisco pescado)
   (es_un_tipo_de salsa condimento)

   (es_un_tipo_de merluza pescado)
   (es_un_tipo_de bacalao pescado)
   (es_un_tipo_de bonito pescado)
   (es_un_tipo_de pulpo marisco)
   (es_un_tipo_de gamba marisco)
   (es_un_tipo_de camaron marisco)
   (es_un_tipo_de almeja marisco)
   (es_un_tipo_de chirla marisco)

   (es_un_tipo_de pato carne_blanca)
   (es_un_tipo_de solomillo carne_roja)
   (es_un_tipo_de carne_picada carne_roja)
   (es_un_tipo_de morcilla embutidos)
   (es_un_tipo_de panceta embutidos)
   (es_un_tipo_de tocino embutidos)
   (es_un_tipo_de jamon embutidos)
   (es_un_tipo_de guanciale embutidos)
   (es_un_tipo_de charqui carne_roja)

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

   (es_un_tipo_de limon fruta)
   (es_un_tipo_de mango fruta)
   (es_un_tipo_de aguacate fruta)
   (es_un_tipo_de coco fruta)
   (es_un_tipo_de fresa fruta)
   (es_un_tipo_de arandano fruta)
   (es_un_tipo_de mandarina fruta)
   (es_un_tipo_de aceituna fruta)

   (es_un_tipo_de mantequilla lacteos)
   (es_un_tipo_de nata lacteos)
   (es_un_tipo_de mascarpone queso)
   (es_un_tipo_de ricotta queso)
   (es_un_tipo_de huevo huevos)

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

   (es_un_tipo_de soja salsa)
   (es_un_tipo_de teriyaki salsa)
   (es_un_tipo_de barbacoa salsa)
   (es_un_tipo_de mostaza salsa)
   (es_un_tipo_de vinagre condimento)
   (es_un_tipo_de levadura condimento)
   (es_un_tipo_de aceite_de_oliva aceite)

   (es_un_tipo_de miel dulces)
   (es_un_tipo_de chocolate dulces)
   (es_un_tipo_de cacao dulces)
)


; Terminos para clasificacion de recetas
(deffacts palabras_clave_tipo_plato
    (palabra_clave salsa "salsa" "alioli" "mayonesa" "vinagreta" "pesto" "bechamel" "romesco" "mojo")
    (palabra_clave bebida "zumo" "jugo" "batido" "smoothie" "limonada" "horchata" "granizado" "infusion" "cafe" "te")
    (palabra_clave postre "tarta" "bizcocho" "galleta" "flan" "helado" "mousse" "brownie" "coulant" "natillas" "cannoli" "torrija" "paparajote")
    (palabra_clave merienda "sandwich" "bocadillo" "tostada" "crepe" "tortitas" "croissant" "napolitana")
    (palabra_clave entrante "ensalada" "salmorejo" "gazpacho" "sopa" "crema" "hummus" "carpaccio" "bruschetta" "tosta" "buñuelos")
    (palabra_clave acompanamiento "guarnicion" "acompañamiento" "pure" "patatas fritas" "patatas asadas" "arroz blanco" "verduras salteadas")
    (palabra_clave principal "espagueti" "macarrones" "solomillo" "lomo" "entrecot" "pizza" "hamburguesa" "lasaña" "paella" "guiso" "potaje")
)

(deffacts terminos_clasificacion_recetas
   (termino_gluten "trigo" "pan" "galleta" "hojaldre" "espagueti" "fideo" "macarron" "pasta" "cuscus" "cerveza")
   (termino_lactosa "leche" "queso" "nata" "mantequilla" "yogur" "mascarpone" "ricotta" "bechamel")
   (termino_picante "guindilla" "chile" "cayena" "jalapeno" "tabasco" "wasabi" "harissa" "curry" "pimenton_picante")
)

(deffacts palabras_clave_grupo_culinario
   (palabra_clave_grupo italiana
      "pizza" "pasta" "risotto" "lasaña" "lasana" "ravioli" "carbonara" "pesto")
   (palabra_clave_grupo oriental
      "sushi" "ramen" "wok" "teriyaki" "curry" "noodles" "fideos chinos" "yakisoba")
   (palabra_clave_grupo mexicana
      "taco" "tacos" "burrito" "quesadilla" "nachos" "fajita" "enchilada" "guacamole")
   (palabra_clave_grupo casera
      "guiso" "potaje" "estofado" "cocido" "lentejas" "albondigas" "albondigas"))


;;; =========================================================
;;; P3 - MAIN
;;; Control de dialogo + activacion del modulo de filtrado
;;; VERSION CON MENU
;;; =========================================================

(deftemplate preferencia
    (slot nombre
        (type SYMBOL)
        (allowed-symbols
            tipo-plato
            vegana
            vegetariana
            sin-gluten
            sin-lactosa
            picante
            calorias
            dificultad-max
            tiempo-max))
    (slot valor))

(deftemplate ingrediente-disponible
    (slot nombre))

(deftemplate modulo
    (slot nombre))

(deftemplate estado-menu
    (slot valor))

(deffacts arranque-p3
   (modulo (nombre preguntar))
   (estado-menu (valor activo))
   (modulos-no-cargados))

;;; ---------------------------------------------------------
;;; Funcion auxiliar:
;;; si la preferencia ya existe, la reemplaza;
;;; si no existe, la crea.
;;; ---------------------------------------------------------
(deffunction set-preferencia (?nombre ?valor)
    ; Busca todos los hechos preferencia que coincidan con ?nombre
    (bind ?hechos (find-all-facts ((?p preferencia)) (eq ?p:nombre ?nombre)))
    (if (> (length$ ?hechos) 0)
        ; Si encuentra preferencias ya introducidas las retracta
        then
            (do-for-all-facts
                ((?p preferencia))
                (eq ?p:nombre ?nombre)
                (retract ?p))
    )

    ; Añadimos la nueva preferencia
    (assert (preferencia (nombre ?nombre) (valor ?valor)))
)

(deffunction reemplaza-todo-main (?txt ?buscar ?reemplazo)
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

(deffunction normaliza-texto-main (?txt)
   (bind ?t (lowcase ?txt))

   (bind ?t (reemplaza-todo-main ?t "á" "a"))
   (bind ?t (reemplaza-todo-main ?t "à" "a"))
   (bind ?t (reemplaza-todo-main ?t "ä" "a"))
   (bind ?t (reemplaza-todo-main ?t "é" "e"))
   (bind ?t (reemplaza-todo-main ?t "è" "e"))
   (bind ?t (reemplaza-todo-main ?t "ë" "e"))
   (bind ?t (reemplaza-todo-main ?t "í" "i"))
   (bind ?t (reemplaza-todo-main ?t "ì" "i"))
   (bind ?t (reemplaza-todo-main ?t "ï" "i"))
   (bind ?t (reemplaza-todo-main ?t "ó" "o"))
   (bind ?t (reemplaza-todo-main ?t "ò" "o"))
   (bind ?t (reemplaza-todo-main ?t "ö" "o"))
   (bind ?t (reemplaza-todo-main ?t "ú" "u"))
   (bind ?t (reemplaza-todo-main ?t "ù" "u"))
   (bind ?t (reemplaza-todo-main ?t "ü" "u"))
   (bind ?t (reemplaza-todo-main ?t "ñ" "n"))

   (bind ?t (reemplaza-todo-main ?t "," " "))
   (bind ?t (reemplaza-todo-main ?t ";" " "))
   (bind ?t (reemplaza-todo-main ?t ":" " "))
   (bind ?t (reemplaza-todo-main ?t "." " "))
   (bind ?t (reemplaza-todo-main ?t "(" " "))
   (bind ?t (reemplaza-todo-main ?t ")" " "))
   (bind ?t (reemplaza-todo-main ?t "/" " "))
   (bind ?t (reemplaza-todo-main ?t "-" " "))
   (bind ?t (reemplaza-todo-main ?t "'" " "))
   (bind ?t (reemplaza-todo-main ?t "\"" " "))

   (while (str-index "  " ?t) do
      (bind ?t (reemplaza-todo-main ?t "  " " ")))

   (bind ?t (reemplaza-todo-main ?t " " "_"))

   (while (str-index "__" ?t) do
      (bind ?t (reemplaza-todo-main ?t "__" "_")))

   (return ?t)
)

(deffunction normaliza-simbolo-main (?txt)
   (return (sym-cat (normaliza-texto-main ?txt)))
)

; Controles logicos para preferencias entre veganos y vegetarianos (vegano => vegetariano)
(defrule MAIN::deducir-vegetariano
    (preferencia (nombre vegana) (valor si))
    =>
    (set-preferencia vegetariana si)
)

(defrule MAIN::deducir-no-vegano
    (preferencia (nombre vegetariana) (valor no))
    =>
    (set-preferencia vegana no)
)

; =======================================================================
; Lista de ingredientes disponibles

(deffunction mostrar-ingredientes ()
    (bind $?ingredientes (find-all-facts ((?i ingrediente-disponible)) TRUE))
    
    (printout t crlf "Ingredientes disponibles actualmente:" crlf)
    
    (if (= (length$ $?ingredientes) 0)
        then
            (printout t "  [ninguno]" crlf)
        else
            (progn$
                (?i $?ingredientes)
                (printout t "  - " (fact-slot-value ?i nombre) crlf)))
)

(deffunction anadir-ingredientes ()
    (printout t crlf "Introduce los ingredientes uno a uno." crlf)
    (printout t "Pulsa Enter en blanco para terminar." crlf)

    (bind ?linea (readline))

    ; Si el primer readline solo recoge el salto pendiente del menu,
    ; hacemos una segunda lectura. Si no, usamos directamente lo que
    ; haya escrito el usuario.
    (if (= (str-length ?linea) 0) then
        (bind ?linea (readline)))

    (while (> (str-length ?linea) 0) do
        (assert (ingrediente-disponible
                    (nombre (normaliza-simbolo-main ?linea))))
        (bind ?linea (readline)))
)
(deffunction borrar-ingrediente (?nombre)
    (bind ?nombre-normalizado (normaliza-simbolo-main (str-cat ?nombre)))

    (bind ?encontrados (find-all-facts
        ((?i ingrediente-disponible))
        (eq ?i:nombre ?nombre-normalizado)))

    (if (> (length$ ?encontrados) 0)
        then
            (do-for-all-facts
                ((?i ingrediente-disponible))
                (eq ?i:nombre ?nombre-normalizado)
                (retract ?i))
            (printout t "Ingrediente eliminado: " ?nombre-normalizado crlf)
        else
            (printout t "Ese ingrediente no estaba en la lista." crlf))
)

(deffunction eliminar-ingredientes ()
    (mostrar-ingredientes)
    (printout t crlf "Escribe el nombre del ingrediente que quieres eliminar." crlf)
    (printout t "Pulsa Enter en blanco para terminar." crlf)

    (bind ?linea (readline))

    ; Igual que en anadir-ingredientes: si la primera lectura es solo
    ; el salto pendiente, leemos otra vez.
    (if (= (str-length ?linea) 0) then
        (bind ?linea (readline)))

    (while (> (str-length ?linea) 0) do
        (borrar-ingrediente ?linea)
        (mostrar-ingredientes)
        (printout t crlf "Escribe el nombre del ingrediente que quieres eliminar." crlf)
        (printout t "Pulsa Enter en blanco para terminar." crlf)
        (bind ?linea (readline)))
)


(defrule MAIN::load-modulos
   (declare (salience 1000))
   ?f <- (modulos-no-cargados)
   =>
   (retract ?f)

   (load "deducir_propiedades.clp")
   (load "propuesta_tipo_receta.clp")
   (load "filtrar.clp")
   (load "presenta.clp")
)

;;; ---------------------------------------------------------
;;; Mostrar menu principal
;;; ---------------------------------------------------------
(defrule MAIN::mostrar-menu
    (declare (salience -1))
    ?m <- (modulo (nombre preguntar))
    ?e <- (estado-menu (valor activo))
    =>
    (retract ?e)

    (printout t crlf "=== ¿QUE COCINO HOY? ===" crlf)
    (printout t "Elige una opcion:" crlf)
    (printout t " 1. Tipo de plato" crlf)
    (printout t " 2. Vegana" crlf)
    (printout t " 3. Vegetariana" crlf)
    (printout t " 4. Sin gluten" crlf)
    (printout t " 5. Sin lactosa" crlf)
    (printout t " 6. Picante" crlf)
    (printout t " 7. Calorias" crlf)
    (printout t " 8. Dificultad maxima" crlf)
    (printout t " 9. Tiempo maximo" crlf)
    (printout t "10. Añadir ingredientes disponibles" crlf)
    (printout t "11. Eliminar ingredientes disponibles" crlf)
    (printout t "12. Continuar" crlf)
    (printout t crlf "Opcion: ")

    (bind ?opcion (read))

    (switch ?opcion
        (case 1 then
            (printout t "Tipo de plato deseado (principal/entrante/postre/merienda/acompanamiento/bebida/salsa/cualquiera): ")
            (bind ?v (read))
            (set-preferencia tipo-plato ?v)
            (assert (estado-menu (valor activo))))

        (case 2 then
            (printout t "¿Debe ser vegana? (si/no/indiferente): ")
            (bind ?v (read))
            (set-preferencia vegana ?v)
            (assert (estado-menu (valor activo))))

        (case 3 then
            (printout t "¿Debe ser vegetariana? (si/no/indiferente): ")
            (bind ?v (read))
            (set-preferencia vegetariana ?v)
            (assert (estado-menu (valor activo))))

        (case 4 then
            (printout t "¿Debe ser sin gluten? (si/no/indiferente): ")
            (bind ?v (read))
            (set-preferencia sin-gluten ?v)
            (assert (estado-menu (valor activo))))

        (case 5 then
            (printout t "¿Debe ser sin lactosa? (si/no/indiferente): ")
            (bind ?v (read))
            (set-preferencia sin-lactosa ?v)
            (assert (estado-menu (valor activo))))

        (case 6 then
            (printout t "¿Te apetece picante? (si/no/indiferente): ")
            (bind ?v (read))
            (set-preferencia picante ?v)
            (assert (estado-menu (valor activo))))

        (case 7 then
            (printout t "Calorias deseadas (ligera/normal/calorica/indiferente): ")
            (bind ?v (read))
            (set-preferencia calorias ?v)
            (assert (estado-menu (valor activo))))

        (case 8 then
            (printout t "Dificultad maxima permitida (facil/media/dificil/indiferente): ")
            (bind ?v (read))
            (set-preferencia dificultad-max ?v)
            (assert (estado-menu (valor activo))))

        (case 9 then
            (printout t "Tiempo maximo de cocinado en minutos (0 = indiferente): ")
            (bind ?v (read))
            (set-preferencia tiempo-max ?v)
            (assert (estado-menu (valor activo))))

        (case 10 then
            (anadir-ingredientes)
            (assert (estado-menu (valor activo))))

        (case 11 then
            (eliminar-ingredientes)
            (assert (estado-menu (valor activo))))

        (case 12 then
            (retract ?m)
            (assert (modulo (nombre deducir-propiedades)))
            (focus DEDUCCION))

        (default
            (printout t "Opcion no valida." crlf)
            (assert (estado-menu (valor activo))))
    )
)

