class CreateZones < ActiveRecord::Migration
  def change
    create_table :zones do |t|
      t.string :name
      t.references :site, index: true, foreign_key: true
      t.integer :numberofslots
      t.integer :numberfree
      t.references :user, index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
