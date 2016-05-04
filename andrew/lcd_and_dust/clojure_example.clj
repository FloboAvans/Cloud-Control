;;example of interfacing with dust sensor from Clojure

(ns farmbot.core
  (:require [riemann.client :as r])
  (import [upm_ppd42ns.PPD42NS]
          [upm_ppd42ns.dustData]
          [mraa.Aio]
          [mraa.Dir])
  (:gen-class))

(def RIEMANN_HOST "192.168.0.25")

(defn -main
  [& args]
  (init-server)
  (println "Starting sensor loop...")
  (let [riemann-client (r/tcp-client {:host RIEMANN_HOST})
        pin0 (mraa.Aio. 0)
        pin1 (mraa.Aio. 1)
        pin2 (mraa.Aio. 2)
        dust (upm_ppd42ns.PPD42NS. 8) ]

    (while true
      (-> riemann-client (r/send-event {:service "aio0" :state "ok" :metric (.read pin0)}))
      (-> riemann-client (r/send-event {:service "aio1" :state "ok" :metric (.read pin1)}))
      (-> riemann-client (r/send-event {:service "aio2" :state "ok" :metric (.read pin2)}))
      ;(let [dust_data (.getData dust)]
      ;  (println "dust concentration: " (.getConcentration dust_data)))
      (Thread/sleep 1000))))
