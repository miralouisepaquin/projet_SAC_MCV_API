module.exports = app => {
    const bois = require("../controllers/bois.controller.js");
    var router = require("express").Router();
    // Create a new Bois
    router.post("/", bois.create);
    // Retrieve all Bois
    router.get("/", bois.findAll);
    // Retrieve a single Bois with id
    router.get("/:id", bois.findOne);
    // Update a Bois with id
    router.put("/:id", bois.update);
    // Delete a Bois with id
    router.delete("/:id", bois.delete);
    // Delete all Bois
    router.delete("/", bois.deleteAll);
    app.use('/api/bois', router);
  };