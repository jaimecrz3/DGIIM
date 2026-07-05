; AUTOR: JAIME CORZO GALDO, MARIO LINDEZ MARTINEZ

(printout t "...Clear..." crlf)
(clear)

(defglobal ?*files* = (create$
  "p6.clp"
  "SE_clasificar_alimentos.clp"
  "BDrecetas_100.clp"
))

(deffunction load-files (?files)
  (foreach ?f ?files
    (printout t "...Cargando " ?f "..." crlf)
    (load ?f)))

(load-files ?*files*)
(reset)

(printout t "...Ejecutando el programa..." crlf)
(run)

; Ejecutar con: (batch* "run.clp")
