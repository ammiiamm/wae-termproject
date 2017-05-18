class CreateParkingslots < ActiveRecord::Migration
  def change
    create_table :parkingslots do |t|
      t.references :zone, index: true, foreign_key: true
      t.string :name
      t.boolean :status
      t.point :point1
      t.point :point2
      t.point :point3
      t.point :point4

      t.timestamps null: false
    end
  end
end
