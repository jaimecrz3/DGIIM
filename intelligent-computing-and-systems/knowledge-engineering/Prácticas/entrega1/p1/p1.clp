; AUTORES: JAIME CORZO GALDO, MARIO LINDEZ MARTINEZ


;;;;; SISTEMA BASADO EN EL CONOCIMIENTO PARA RECOMENDAR LA CANTIDAD A TOMAR DE UN ALIMENTO  ;;;;;
                  ;;;;; PARA MANTENER UNA DIETA CARDIOSALUDABLE ;;;;;;;;;
				
;;; Fuente de conocimiento: https://fundaciondelcorazon.com/nutricion/piramide-de-alimentacion.html				
				  

;;;ENTRADAS;;;
;;;; (alimento ?a) representará ?a es el alimento sobre el que se pide información
; por ejemplo cuando esté (alimento pan) representará que el usuario desea información sobre el pan

;;; HECHOS DE CONOCIMIENTO
;;;; (es_un_tipo_de ?x ?y) representará ?x es un tipo de ?y
; por ejemplo "los macarrones son un tipo de pasta" se representará (es_un_tipo_de macarrones pasta)
; se introducen de forma explícita algunos, otros se deducen

;;;; (nivel_piramide_alimentaria ?a ?n) representará ?a es un tipo de alimento del nivel ?n de la pirámide alimentaria
; todos se introducen de forma explícita

;;;; (propiedad ?p ?a ?v) representará el valor de la propiedad ?p para ?a es ?v 
; por ejemplo, "la pasta en un alimento rico en hidratos de carbono" se representará (propiedad rico_en_hidratos pasta si)
; se introducen algunos, otros se deducen

(defglobal ?*contador* = 0)

(deffacts piramide_alimentaria
(nivel_piramide_alimentaria verdura 1)
(nivel_piramide_alimentaria hortalizas 1)
(nivel_piramide_alimentaria fruta 2)
(nivel_piramide_alimentaria cereales 3)
(nivel_piramide_alimentaria lacteos 4)
(nivel_piramide_alimentaria aceite_de_oliva 5)
(nivel_piramide_alimentaria frutos_secos 6)
(nivel_piramide_alimentaria especies 6)
(nivel_piramide_alimentaria hierbas_aromaticas 6)
(nivel_piramide_alimentaria legumbres 7)
(nivel_piramide_alimentaria carne_blanca 8)
(nivel_piramide_alimentaria pescado 8)
(nivel_piramide_alimentaria huevos 8)
(nivel_piramide_alimentaria carne_roja 9)
(nivel_piramide_alimentaria embutidos 9)
(nivel_piramide_alimentaria fiambres 9)
(nivel_piramide_alimentaria dulces 9)
)


(deffacts subtipos
(es_un_tipo_de carne_roja carne)
(es_un_tipo_de ternera carne_roja)
(es_un_tipo_de cerdo carne_roja)
(es_un_tipo_de cordero carne_roja)
(es_un_tipo_de carne_blanca carne)
(es_un_tipo_de pollo carne_blanca)
(es_un_tipo_de conejo carne_blanca)
(es_un_tipo_de leche lacteos)
(es_un_tipo_de queso lacteos)
(es_un_tipo_de yogur lacteos)
(es_un_tipo_de atun pescado)   
(es_un_tipo_de boquerones pescado)
(es_un_tipo_de sardinas pescado)
(es_un_tipo_de salchichon embutidos)
(es_un_tipo_de chorizo embutidos)
(es_un_tipo_de judias_blancas legumbres)
(es_un_tipo_de garbanzos legumbres)
(es_un_tipo_de guisantes legumbres)
(es_un_tipo_de nueces frutos_secos)
(es_un_tipo_de almendra frutos_secos)
(es_un_tipo_de perejil hierbas_aromaticas)
(es_un_tipo_de pimienta especies)
(es_un_tipo_de pimenton especies)
(es_un_tipo_de cereales_integrales cereales)
(es_un_tipo_de trigo cereales)
(es_un_tipo_de harina cereales)
(es_un_tipo_de maiz cereales)
(es_un_tipo_de sandia fruta)
(es_un_tipo_de pinia fruta)
(es_un_tipo_de platano fruta)
(es_un_tipo_de pera fruta)
(es_un_tipo_de manzana fruta)
(es_un_tipo_de naranja fruta)
(es_un_tipo_de lechuga verdura)
(es_un_tipo_de coliflor verdura)
(es_un_tipo_de brocoli verdura)
(es_un_tipo_de ajo verdura)
(es_un_tipo_de pimiento verdura)
(es_un_tipo_de zanahoria verdura)
(es_un_tipo_de cebolla verdura)
(es_un_tipo_de tomate verdura)
(es_un_tipo_de pimiento_rojo pimiento)
(es_un_tipo_de pimiento_verde pimiento)
(es_un_tipo_de pastel dulces)
(es_un_tipo_de caramelos dulces)
(es_un_tipo_de azucar dulces)
)


(deffacts cantidad_recomendada
(cantidad_recomendada nivel 1 "en raciones de 120-150 gramos" "3-4 veces al dia")
(cantidad_recomendada nivel 2 "en raciones de 150-200 gramos" "2-3 veces al dia")
(cantidad_recomendada nivel 3 "en raciones de 50 gramos" "3 veces al dia")
(cantidad_recomendada nivel 4 "en raciones de 120-150 gramos" "1-2 veces al dia")
(cantidad_recomendada nivel 5 "" "4 cucharadas al dia")
(cantidad_recomendada nivel 6 "en raciones de 25-30 gramos" "1 vez al dia")
(cantidad_recomendada nivel 7 "" "3 veces a la semana")
(cantidad_recomendada nivel 8 "" "3-5 dias a la semana")
(cantidad_recomendada nivel 9 "" "de forma esporadica")
)

(deffacts hecho_de
(compuesto_fundamentalmente_por pan harina)
(compuesto_fundamentalmente_por pasta harina)
(compuesto_fundamentalmente_por pizza harina)
)

(defrule componiendo_es_un_tipo_de
   (declare (salience 5))
   (es_un_tipo_de ?x ?y)
   (es_un_tipo_de ?y ?z)
   =>
   (assert (es_un_tipo_de ?x ?z))
)

(defrule compuesto_fundamentalmente_por_entonces_es_un_tipo_de
(declare (salience 6))
(compuesto_fundamentalmente_por ?x ?y)
=>
(assert (es_un_tipo_de ?x ?y))
)


(defrule indicar_nivel_grupo
(alimento ?a)
(nivel_piramide_alimentaria ?a ?n)
=>
(printout t crlf "Pertence al nivel " ?n " de la cadena alimentaria, compuesto por: ")
(assert (nivel ?n))
)

(defrule indicar_nivel_por_tipo
(alimento ?a)
(es_un_tipo_de ?a ?x)
(nivel_piramide_alimentaria ?x ?n)
=>
(printout t crlf "Pertence al nivel " ?n " de la cadena alimentaria, compuesto por: ")
(assert (nivel ?n))
)

(defrule indicar_compuesto_fundamentalmente_por
(declare (salience 2))
(alimento ?a)
(compuesto_fundamentalmente_por ?a ?b)
=>
(printout t crlf "Esta compuesto fundamentalmente por " ?b ", asi que lo clasificaremos como este alimento" crlf)
)

(defrule recomendar_cereales_siempre_integrales
(alimento ?a)
(es_un_tipo_de ?a cereales)
=>
(printout t crlf crlf "Se recomienda que los cereales se tomen siempre integrales")
)

(defrule recomendar_cereales_siempre_integrales2
(alimento cereales)
=>
(printout t crlf crlf "Se recomienda que los cereales se tomen siempre integrales")
)

(defrule recomendar_lacteos_desnatados_o_semidenatados
(alimento ?a)
(es_un_tipo_de ?a lacteos)
=>
(printout t crlf crlf "Se recomienda que los lacteos se tomen desnatados o semidesnatados")
)

(defrule recomendar_lacteos_desnatados_o_semidenatados2
(alimento lacteos)
=>
(printout t crlf crlf "Se recomienda que los lacteos se tomen desnatados o semidesnatados")
)

(defrule describir_nivel
(nivel ?n)
(nivel_piramide_alimentaria ?a ?n)
=>
(printout t "- " ?a "   ")
)


(defrule indicar_cantidad_recomendada
(declare (salience -1))
(nivel ?n)
(cantidad_recomendada nivel ?n ?t1 ?t2)
=>
(printout t crlf crlf "Para el conjunto de los alimentos de este nivel se recomienda consumirlos " ?t1 " " ?t2 crlf crlf)
)

(defrule preguntar_alimento
=>
(printout t crlf "Indica el alimento del que deseas saber la cantidad recomendada: " )
(assert (alimento (read)))
)

;;;;;;;; AMPLIANDO EL SISTEMA ;;;;;;;
(deffacts rico_en_proteinas
(propiedad rico_en_proteinas carne)
(propiedad rico_en_proteinas pescado)
(propiedad rico_en_proteinas huevos)
(propiedad rico_en_proteinas lacteos)
)

(deffacts rico_en_hidratos_de_carbono
(propiedad rico_en_hidratos cereales)
(propiedad rico_en_hidratos frutos_secos)
(propiedad rico_en_hidratos legumbres)
)

(deffacts rico_en_fibras
(propiedad rico_en_fibras fruta)
(propiedad rico_en_fibras verdura)
(propiedad rico_en_fibras hortalizas)
(propiedad rico_en_fibras hortalizas)
)

(deffacts rico_en_grasas
(propiedad rico_en_grasas carne_roja)
(propiedad rico_en_grasas embutidos)
(propiedad rico_en_grasas aceite_de_oliva)
(propiedad rico_en_grasas queso)  
)

(deffacts rico_en_azucares
(propiedad rico_en_azucares dulces)
(propiedad rico_en_azucares fruta)
)

(deffacts alimento_saludable
(propiedad alimento_saludable verdura)
(propiedad alimento_saludable hortalizas)
(propiedad alimento_saludable fruta)
(propiedad alimento_saludable aceite_de_oliva)
(propiedad alimento_saludable frutos_secos)
(propiedad alimento_saludable legumbres)
(propiedad alimento_saludable carne_blanca)
(propiedad alimento_saludable pescado)
(propiedad alimento_saludable huevos)
)

(defrule herencia_propiedades
(propiedad ?p ?a)
(es_un_tipo_de ?x ?a)
=>
(assert (propiedad ?p ?x))
)

(defrule indicar_propiedad_por_tipo
(declare (salience 1))
(alimento ?a)
(es_un_tipo_de ?a ?x)
(nivel_piramide_alimentaria ?x ?)
(propiedad ?p ?x)
=>
(printout t crlf "Este es un alimento " ?p " porque es un tipo de " ?x crlf)
)

; (defrule preguntar_alimento_propiedades
; (declare (salience -2))
; =>
; (printout t crlf "Indica el alimento del que quieres conocer sus propiedades:" crlf)
; (assert (alimento_propiedades (read)))
; )

; (defrule listar_propiedades_alimento
; (declare (salience -3))
; (alimento ?a)
; (propiedad ?p ?a)
; =>
; (printout t crlf "Propiedad del alimento " ?a ": " ?p crlf)
; )

;;;;;; EJERCICIO PARTE 1:  AÑADIR REGLAS PARA LISTAR LOS ALIMENTOS DE LOS QUE SE DISPONE DE INFORMACION ANTES DE PREGUNTAR
;;; Indicaciones: 1) deduce hechos (es_alimento ?x) representando que algo es un alimento a partir de la relacion "es_un_tipo_de" ¡Ojo serían alimentos, no tipos de alimentos!
;;;               2) Imprime por pantalla los es_alimento

;;;;;; EJERCICIO PARTE 2:  AÑADIR REGLAS PARA INDICAR AL FINAL OTROS ALIMENTOS CON LAS MISMAS PROPIEDADES
;;; Indicaciones: 1) deduce (alimento_parecido ?x) para los alimentos que tengan alguna propiedad común con el alimento sobre el que se pregunta
;;;               2) retracta los alimento_parecido que tengan una propiedad distinta con el preguntado
;;;               3) Imprime por pantalla los alimento_parecido que queden 

;;;
;;;;;; EJERCICIO 1
;;;
(defrule def_alimento
    (declare (salience 4))
    (es_un_tipo_de ?x ?y)
    =>
    (assert (es_alimento ?x))
)


(defrule listar_alimentos
   (declare (salience 3))
   (es_alimento ?x)
   =>
   (bind ?*contador* (+ ?*contador* 1))
   (printout t ?*contador* ". " ?x crlf)
)

;;;
;;;;;; EJERCICIO 2
;;;

(defrule obtener_todos_alimentos_parecidos
    (declare (salience -5))
    (alimento ?x)
    (es_alimento ?y&~?x)
    (propiedad ?p ?x)
    (propiedad ?p ?y)
    =>
    (assert (alimento_parecido ?y))

)

(defrule retractar_parecidos_con_propiedad_distinta
    (declare (salience -6))
    (alimento ?x)
    ?f <- (alimento_parecido ?y)
    (propiedad ?p ?y)
    (not (propiedad ?p ?x))
    =>
    (retract ?f)
)

(defrule imprimir_parecidos
    (declare (salience -7))
    (alimento ?x)
    (alimento_parecido ?y)
    =>
    (printout t crlf "Alimento parecido a " ?x ": " ?y crlf)
)