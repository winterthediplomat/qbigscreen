for windid in $(xdotool search --pid $1); do
    echo $windid;
    xdotool windowactivate $windid;
done;
