(function () {
    const leftButton = document.createElement("div");
    leftButton.setAttribute("class", "button");

    const rightButton = document.createElement("div");
    rightButton.setAttribute("class", "button");
  
    const mainContent = document.getElementById('main');
    mainContent.appendChild(leftButton);
    mainContent.appendChild(rightButton);

    const drive = (motorSide, shouldDrive) => fetch(`/motors/${motorSide}?drive=${shouldDrive}`);

    leftButton.addEventListener("touchstart", () => drive("left", true));
    leftButton.addEventListener("touchend", () => drive("left", false));

    rightButton.addEventListener("touchstart", () => drive("right", true));
    rightButton.addEventListener("touchend", () => drive("right", false));
})();