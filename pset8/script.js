$('.carousel').carousel({
    interval: 3000
  })

  function test() {
    alert("Hi!");
  }

  // Simple js animation to cause all elements to rise (doesnt account for prior appearance on the screen,
  // or for overflow handling, but this website was made quickly for proof of concept)
  document.addEventListener('DOMContentLoaded', () => {
    const all = document.getElementsByTagName("*");
    for (let i = 0; i < all.length; i++) {
        // if (all[i].id == "loader") { continue; }
        all[i].style.animation = "generate 1s ease";
    }
  });