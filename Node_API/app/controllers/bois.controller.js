const db = require("../models");
const Bois = db.bois;
// Create and Save a new Bois
exports.create = (req, res) => {
  // Validate request
  if (!req.body.nom) {
    res.status(400).send({ message: "Content can not be empty!" });
    return;
  }
  // Create a Bois
  const bois = new Bois({
    nom: req.body.nom,
    type: req.body.type,
    origine: req.body.origine,
    tempsSechage: req.body.tempsSechage,
    temperature_min: req.body.temperature_min
  });
  // Save Bois in the database
  bois
    .save(bois)
    .then(data => {
      res.send(data);
    })
    .catch(err => {
      res.status(500).send({
        message:
          err.message || "Some error occurred while creating the Bois."
      });
    });
};
// Retrieve all Bois from the database.
exports.findAll = (req, res) => {
    const nom = req.query.nom;
    var condition = nom ? { nom: { $regex: new RegExp(nom), $options: "i" } } : {};
    Bois.find(condition)
      .then(data => {
        res.send(data);
      })
      .catch(err => {
        res.status(500).send({
          message:
            err.message || "Some error occurred while retrieving tutorials."
        });
      });
};
// Find a single Bois with an id
exports.findOne = (req, res) => {
    const id = req.params.id;
    Bois.findById(id)
      .then(data => {
        if (!data)
          res.status(404).send({ message: "Not found Bois with id " + id });
        else res.send(data);
      })
      .catch(err => {
        res
          .status(500)
          .send({ message: "Error retrieving Bois with id=" + id });
      });
};
// Update a Bois by the id in the request
exports.update = (req, res) => {
    if (!req.body) {
        return res.status(400).send({
          message: "Data to update can not be empty!"
        });
      }
      const id = req.params.id;
      Bois.findByIdAndUpdate(id, req.body, { useFindAndModify: false })
        .then(data => {
          if (!data) {
            res.status(404).send({
              message: `Cannot update Bois with id=${id}. Maybe Tutorial was not found!`
            });
          } else res.send({ message: "Bois was updated successfully." });
        })
        .catch(err => {
          res.status(500).send({
            message: "Error updating Bois with id=" + id
          });
        });
};
// Delete a Bois with the specified id in the request
exports.delete = (req, res) => {
    const id = req.params.id;
    Bois.findByIdAndRemove(id)
      .then(data => {
        if (!data) {
          res.status(404).send({
            message: `Cannot delete Bois with id=${id}. Maybe Bois was not found!`
          });
        } else {
          res.send({
            message: "Bois was deleted successfully!"
          });
        }
      })
      .catch(err => {
        res.status(500).send({
          message: "Could not delete Bois with id=" + id
        });
      });
};
// Delete all Bois from the database.
exports.deleteAll = (req, res) => {
    Bois.deleteMany({})
    .then(data => {
      res.send({
        message: `${data.deletedCount} Bois were deleted successfully!`
      });
    })
    .catch(err => {
      res.status(500).send({
        message:
          err.message || "Some error occurred while removing all Bois."
      });
    });
};
