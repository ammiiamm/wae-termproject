class CreateSites < ActiveRecord::Migration
  def change
    create_table :sites do |t|
      t.string :name
      t.string :address
      t.string :districs
      t.string :city
      t.string :postalcode
      t.references :user, index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
