module.exports = mongoose => {
    var schema = mongoose.Schema(
        {
            nom: String,
            type: String,
            origine: String,
            tempsSechage: String,
            temperature_min: String
        },
        { timestamps: true }
    );
    schema.method("toJSON", function() {
        const { __v, _id, ...object } = this.toObject();
        object.id = _id;
        return object;
      });
      const Bois = mongoose.model("bois", schema);
    return Bois;
  };
