class CreateAdvertisements < ActiveRecord::Migration
  def change
    create_table :advertisements do |t|
      t.string :add_type
      t.string :contecnt
      t.boolean :status
      t.references :user, index: true, foreign_key: true
      t.references :site, index: true, foreign_key: true
      t.references :zone, index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
