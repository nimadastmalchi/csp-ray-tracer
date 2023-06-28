if [ $# -ne 1 ]; then
  echo "Usage: $0 <number>"
  exit 1
fi

case $1 in
  1)
    gnuplot -p -e "splot './Data/~miss_panel.txt' with vectors lw 1 lt 5, './Data/~hit_panel.txt' with vectors lw 1 lt 10, './Data/~hit_collector.txt' with vectors lw 1 lt 10, './Data/~miss_collector.txt' with vectors lw 1 lt 1, './Data/~collector_lines.txt' with lines lt -1, './Data/~panel_lines.txt' with vectors nohead lw 1 lt - 1 , './Data/~sun_path.txt' with vectors nohead lw 1 lt -1"
    ;;
  2)
    gnuplot -p -e "splot './Data/~hit_collector.txt' with vectors lw 1 lt 10, './Data/~miss_collector.txt' with vectors lw 1 lt 1, './Data/~collector_lines.txt' with lines lt -1, './Data/~panel_lines.txt' with vectors nohead lw 1 lt - 1 , './Data/~sun_path.txt' with vectors nohead lw 1 lt -1"
    ;;
  3)
    gnuplot -p -e "splot './Data/~panel_lines.txt' with vectors nohead lw 1 lt - 1"
    ;;
  4)
    gnuplot -p -e "splot './Data/~sun_path.txt' with vectors nohead lw 1 lt -1"
    ;;
  5)
    gnuplot -p -e "plot './Data/~var_sun_power.txt' with point pt 7 smooth csplines"
    ;;
  6)
    gnuplot -p -e "plot './Data/~var_sun_temp.txt' with point pt 7 smooth csplines"
    ;;
  7)
    gnuplot -p -e "plot './Data/~var_sun_inc_power.txt' with point pt 7 smooth csplines"
    ;;
  8)
    gnuplot -p -e "plot './Data/~var_sun_power_fixed.txt' with point pt 7 smooth csplines"
    ;;
  9)
    gnuplot -p -e "plot './Data/~var_sun_temp_fixed.txt' with point pt 7 smooth csplines"
    ;;
  *)
    echo "Invalid number: $1"
    exit 1
    ;;
esac


