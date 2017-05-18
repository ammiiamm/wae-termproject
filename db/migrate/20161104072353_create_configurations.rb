class CreateConfigurations < ActiveRecord::Migration
  def change
    create_table :configurations do |t|
      t.string :name
      t.boolean :config_status
      t.references :site, index: true, foreign_key: true
      t.references :user, index: true, foreign_key: true
      t.references :camera, index: true, foreign_key: true
      t.references :zone, index: true, foreign_key: true
      t.string :slotconfiguration
      t.string :roihomo

      t.timestamps null: false
    end
  end
end
