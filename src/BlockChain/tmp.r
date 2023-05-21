for(deseo in 1:5){
  for(ciclo in 1:10){
    apply(DF, 1, function(r) any(r %in% c("M017", "M018")))
    # if(!(deseo %in% mockup_freq$DESEO ) | !(ciclo %in% mockup_freq$CICLO)){
  }

}
