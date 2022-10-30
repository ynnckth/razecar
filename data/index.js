(function () {
    const LEFT_MOTORS_SIDE = "left";
    const RIGHT_MOTORS_SIDE = "right";

    const leftButton = document.createElement("div");
    leftButton.setAttribute("class", "button inactive");

    const rightButton = document.createElement("div");
    rightButton.setAttribute("class", "button inactive");
  
    const mainContent = document.getElementById('main');
    mainContent.appendChild(leftButton);
    mainContent.appendChild(rightButton);

    const drive = (motorSide, shouldDrive) => {
        fetch(`/motors/${motorSide}?drive=${shouldDrive}`);

        currentButton = motorSide === LEFT_MOTORS_SIDE ? leftButton : rightButton;
        if (shouldDrive) {
            currentButton.setAttribute("class", "button active");
        } else {
            currentButton.setAttribute("class", "button inactive");
        }
    };

    leftButton.addEventListener("touchstart", () => drive(LEFT_MOTORS_SIDE, true));
    leftButton.addEventListener("touchend", () => drive(LEFT_MOTORS_SIDE, false));

    rightButton.addEventListener("touchstart", () => drive(RIGHT_MOTORS_SIDE, true));
    rightButton.addEventListener("touchend", () => drive(RIGHT_MOTORS_SIDE, false));
})();